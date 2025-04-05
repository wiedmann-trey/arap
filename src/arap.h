#pragma once

#include "graphics/shape.h"
#include "Eigen/StdList"
#include "Eigen/StdVector"
#include "Eigen/Sparse"
#include <optional>

class Shader;

class ARAP
{
private:
    Shape m_shape;

    std::vector<Eigen::Vector3f> m_vertices;
    std::vector<Eigen::Vector3f> m_new_vertices;
    std::vector<std::unordered_set<int>> m_adjacent;
    std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> m_vert_opposite_edge;

    std::unordered_map<int, std::unordered_map<int, float>> m_edge_weights;
    Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> m_factorization;

    std::vector<Eigen::Matrix3f> m_rotations;

public:
    ARAP();

    void init(Eigen::Vector3f &min, Eigen::Vector3f &max);
    void move(int vertex, Eigen::Vector3f pos);
    void startDragging();
    void finishDragging();

    // ================== Students, If You Choose To Modify The Code Below, It's On You

    int getClosestVertex(Eigen::Vector3f start, Eigen::Vector3f ray, float threshold)
    {
        return m_shape.getClosestVertex(start, ray, threshold);
    }

    void draw(Shader *shader, GLenum mode)
    {
        m_shape.draw(shader, mode);
    }

    SelectMode select(Shader *shader, int vertex)
    {
        return m_shape.select(shader, vertex);
    }

    bool selectWithSpecifiedMode(Shader *shader, int vertex, SelectMode mode)
    {
        return m_shape.selectWithSpecifiedMode(shader, vertex, mode);
    }

    bool getAnchorPos(int lastSelected, Eigen::Vector3f& pos, Eigen::Vector3f ray, Eigen::Vector3f start)
    {
        return m_shape.getAnchorPos(lastSelected, pos, ray, start);
    }

};
