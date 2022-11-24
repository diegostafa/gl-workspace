#ifndef entity_cpp
#define entity_cpp

#include "../utils/texture_tools.cpp"
#include "mesh.cpp"
#include "shader.cpp"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class entity {
public:
  std::vector<texture> textures_loaded;
  std::vector<mesh> meshes;
  std::string directory;
  glm::mat4 model = glm::mat4(1.0f);

  void move_to(glm::vec3 dest) {
    model = glm::translate(glm::identity<glm::mat4>(), dest);
  }
  void scale(glm::vec3 factor) {
    model = glm::scale(glm::identity<glm::mat4>(), factor);
  }

  entity(std::string const &path) { load_model(path); }

  void draw(shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
      meshes[i].render(shader);
    }
  }

  void load_model(std::string const &path) {
    Assimp::Importer importer;

    auto scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                  aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || !scene->mRootNode ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
      std::cout << "ERROR @ entity::loadModel(path): " << std::endl;
      exit(1);
    }

    directory = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode, scene);
  }

  void process_node(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      meshes.push_back(process_mesh(scene->mMeshes[node->mMeshes[i]], scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      process_node(node->mChildren[i], scene);
    }
  }

  mesh process_mesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      vertex vertex;

      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                  mesh->mVertices[i].z);

      if (mesh->HasNormals()) {
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                  mesh->mNormals[i].z);
      }
      if (mesh->mTextureCoords[0]) {
        vertex.tex_coord = glm::vec2(mesh->mTextureCoords[0][i].x,
                                     mesh->mTextureCoords[0][i].y);
        vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y,
                                   mesh->mTangents[i].z);
        vertex.bi_tangent =
            glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y,
                      mesh->mBitangents[i].z);
      } else {
        vertex.tex_coord = glm::vec2(0.0f, 0.0f);
      }
      vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<texture> diffuseMaps = load_material_textures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<texture> specularMaps = load_material_textures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    std::vector<texture> normalMaps = load_material_textures(
        material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    std::vector<texture> heightMaps = load_material_textures(
        material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    class mesh m(vertices, textures, indices);
    return m;
  }

  std::vector<texture> load_material_textures(aiMaterial *mat,
                                              aiTextureType type,
                                              const std::string &typeName) {
    std::vector<texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);
      bool skip = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
          textures.push_back(textures_loaded[j]);
          skip = true;
          break;
        }
      }
      if (!skip) {
        texture texture;
        texture.id = TextureFromFile(str.C_Str(), this->directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        textures_loaded.push_back(texture);
      }
    }
    return textures;
  }
};

#endif