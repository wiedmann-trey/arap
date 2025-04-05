#include "arap.h"
#include "graphics/meshloader.h"

#include <iostream>
#include <set>
#include <map>
#include <vector>

using namespace std;
using namespace Eigen;

float cotangent(Eigen::Vector3f a, Eigen::Vector3f b) {
    float cosine = a.dot(b) / (a.norm()*b.norm());
    return std::abs(cosine / std::sqrt(1 - cosine * cosine));
}

ARAP::ARAP() {}

void ARAP::init(Eigen::Vector3f &coeffMin, Eigen::Vector3f &coeffMax)
{
    vector<Vector3f> vertices;
    vector<Vector3i> triangles;

    // If this doesn't work for you, remember to change your working directory
    if (MeshLoader::loadTriMesh("meshes/peter.obj", vertices, triangles)) {
        m_shape.init(vertices, triangles);
    }

    // Students, please don't touch this code: get min and max for viewport stuff
    MatrixX3f all_vertices = MatrixX3f(vertices.size(), 3);
    int i = 0;
    for (unsigned long i = 0; i < vertices.size(); ++i) {
        all_vertices.row(i) = vertices[i];
    }
    coeffMin = all_vertices.colwise().minCoeff();
    coeffMax = all_vertices.colwise().maxCoeff();

    m_vertices = vertices;
    m_adjacent.resize(m_vertices.size());

    for(Vector3i &tri : triangles) {
        m_adjacent[tri[0]].insert(tri[1]);
        m_adjacent[tri[1]].insert(tri[0]);
        m_adjacent[tri[1]].insert(tri[2]);
        m_adjacent[tri[2]].insert(tri[1]);
        m_adjacent[tri[2]].insert(tri[0]);
        m_adjacent[tri[0]].insert(tri[2]);

        m_vert_opposite_edge[tri[0]][tri[1]].insert(tri[2]);
        m_vert_opposite_edge[tri[1]][tri[0]].insert(tri[2]);
        m_vert_opposite_edge[tri[1]][tri[2]].insert(tri[0]);
        m_vert_opposite_edge[tri[2]][tri[1]].insert(tri[0]);
        m_vert_opposite_edge[tri[2]][tri[0]].insert(tri[1]);
        m_vert_opposite_edge[tri[0]][tri[2]].insert(tri[1]);
    }

    m_rotations.resize(m_vertices.size());
}

// Move an anchored vertex, defined by its index, to targetPosition
void ARAP::move(int vertex, Vector3f targetPosition)
{
    const std::unordered_set<int>& anchors = m_shape.getAnchors();
    int n = m_vertices.size();

    for(int step = 0; step < 2; step++) {

        /* OPTIMIZE ROTATIONS */
        // to find rotation
        // build S
        // SVD of S -> gives us rotations

        #pragma omp parallel for
        for(int i = 0; i < n; i++) {
            Matrix3f S = Eigen::Matrix3f::Zero();
            for(const int &j : m_adjacent[i]) {
                Vector3f e = m_vertices[i] - m_vertices[j];
                Vector3f e_prime = m_new_vertices[i] - m_new_vertices[j];
                float w = m_edge_weights[i][j];

                S += w*e*e_prime.transpose();
            }

            JacobiSVD<Matrix3f> svd(S, Eigen::ComputeFullU | Eigen::ComputeFullV);
            Matrix3f U = svd.matrixU();
            Matrix3f V = svd.matrixV();


            if((V*U.transpose()).determinant() < 0) {
                int min_index;
                svd.singularValues().minCoeff(&min_index);

                U.col(min_index) *= -1;
            }

            m_rotations[i] = V*U.transpose();
        }

        /* OPTIMIZE POINTS */
        // build b matrix, use factorization to solve for points

        VectorXf b_x(n);
        VectorXf b_y(n);
        VectorXf b_z(n);

        #pragma omp parallel for
        for(int i = 0; i < n; i++) {
            if(i == vertex) {
                b_x[i] = targetPosition[0];
                b_y[i] = targetPosition[1];
                b_z[i] = targetPosition[2];
                continue;
            }

            if(anchors.contains(i)) {
                b_x[i] = m_vertices[i][0];
                b_y[i] = m_vertices[i][1];
                b_z[i] = m_vertices[i][2];
                continue;
            }

            b_x[i] = 0;
            b_y[i] = 0;
            b_z[i] = 0;
            for(const int &j : m_adjacent[i]) {
                Vector3f v(0.f,0.f,0.f);
                float w = m_edge_weights[i][j];

                if(j == vertex) {
                    v = w*targetPosition;
                } else if(anchors.contains(j)) {
                    v = w*m_vertices[j];
                }
                v += (w/2) * (m_rotations[i] + m_rotations[j]) * (m_vertices[i] - m_vertices[j]);

                b_x[i] += v[0];
                b_y[i] += v[1];
                b_z[i] += v[2];
            }
        }

        VectorXf new_x;
        VectorXf new_y;
        VectorXf new_z;

        // back substituting in parallel
        #pragma omp parallel for
        for(int i = 0; i < 3; i++) {
            if(i==0) {
                new_x = m_factorization.solve(b_x);
                continue;
            }

            if(i==1) {
                new_y = m_factorization.solve(b_y);
                continue;
            }

            if(i==2) {
                new_z = m_factorization.solve(b_z);
                continue;
            }
        }

        #pragma omp parallel for
        for(int i = 0; i < n; i++) {
            m_new_vertices[i][0] = new_x[i];
            m_new_vertices[i][1] = new_y[i];
            m_new_vertices[i][2] = new_z[i];
        }
    }

    m_shape.setVertices(m_new_vertices);

    // Here are some helpful controls for the application
    //
    // - You start in first-person camera mode
    //   - WASD to move, left-click and drag to rotate
    //   - R and F to move vertically up and down
    //
    // - C to change to orbit camera mode
    //
    // - Right-click (and, optionally, drag) to anchor/unanchor points
    //   - Left-click an anchored point to move it around
    //
    // - Minus and equal keys (click repeatedly) to change the size of the vertices
}


void ARAP::startDragging() {
    // initialize weights
    // n by n, weight from i to j, symmetric
    // weight = 1/2 * sum of cotangents angles opposite of that edge

    #pragma omp parallel for
    for(int i = 0; i < m_vertices.size(); i++) {
        for(const int &j : m_adjacent[i]) {
            float w = 0;
            for(const int &k : m_vert_opposite_edge[i][j]) {
                w += .5*cotangent(m_vertices[i]-m_vertices[k], m_vertices[j]-m_vertices[k]);
            }
            m_edge_weights[i][j] = w;
            m_edge_weights[j][i] = w;
        }
    }

    // set new positions p' to current positions
    m_new_vertices = m_vertices;

    // build L matrix
    // L is n vertices by n vertices
    // for row i, column i is sum of weights row i, other columns are negative of that weight
    int n = m_vertices.size();
    Eigen::SparseMatrix<float> L(n,n);
    const std::unordered_set<int>& anchors = m_shape.getAnchors();

    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        if(anchors.contains(i)) {
            L.insert(i,i) = 1.f;
            continue;
        }

        float weight_sum = 0.f;
        for(const int &j : m_adjacent[i]) {
            float w = m_edge_weights[i][j];
            weight_sum += w;
            if(anchors.contains(j)) continue;
            L.insert(i,j) = -w;
        }
        L.insert(i,i) = weight_sum;
    }

    L.makeCompressed();

    // compute cholesky factorization
    m_factorization.analyzePattern(L);
    m_factorization.factorize(L);
}

void ARAP::finishDragging() {
    m_vertices = m_new_vertices;
}
