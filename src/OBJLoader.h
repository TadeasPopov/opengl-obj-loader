#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

namespace OBJLoader {
	struct OBJData
	{
		std::vector<float> values;
		int vertices_count;
		int uv_coordinates_count;
		int normals_count;
	};

	OBJData LoadOBJ(std::string path)
	{
		std::ifstream stream(path);

		if (!stream.is_open())
		{
			std::cerr << "File: " << path << " does not exist." << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string line;
		std::smatch matches;

		std::regex regex_mtllib("\\s*mtllib\\s+(\\w+\\.mtl)\\s*");
		std::regex regex_usemtl("\\s*usemtl\\s+(\\w+)\\s*");

		std::regex regex_object_name("\\s*o\\s+(\\w+)\\s*");
		std::regex regex_group_name("\\s*g\\s+(\\w+)\\s*");

		// x: g1, y: g3, z: g5, w: g9
		std::regex regex_vertex("\\s*v\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)((\\s+(-?\\d+(\\.\\d+)?)\\s*)|(\\s*))");
		// u: g1, v: g5, w: g9
		std::regex regex_uv_coordinate("\\s*vt\\s+(-?\\d+(\\.\\d+)?)((\\s+(-?\\d+(\\.\\d+)?)\\s*)|(\\s*))((\\s+(-?\\d+(\\.\\d+)?)\\s*)|(\\s*))");
		// x: g1, y: g3, z: g5
		std::regex regex_normal("\\s*vn\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)\\s*");
		// u: g1, v: g5, w: g9
		std::regex regex_space_vertex("\\s*vp\\s+(-?\\d+(\\.\\d+)?)((\\s+(-?\\d+(\\.\\d+)?)\\s*)|(\\s*))((\\s+(-?\\d+(\\.\\d+)?)\\s*)|(\\s*))");

		// vi - vertex indice
		// ui - uv coordinate indice
		// ni - normal indice
		std::regex regex_face_vi("\\s*f\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*");
		std::regex regex_face_vi_ui("\\s*f\\s+(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\s*");
		std::regex regex_face_vi_ui_ni("\\s*f\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s*");
		std::regex regex_face_vi_ni("\\s*f\\s+(\\d+)\\/\\/(\\d+)\\s+(\\d+)\\/\\/(\\d+)\\s+(\\d+)\\/\\/(\\d+)\\s*");

		OBJData data;
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> uv_coordinates;
		/* vector<string> mtllib_files_paths;
		vector<string> usemtl_names;
		MeshObject* last_mesh_object = nullptr; */

		while (getline(stream, line))
		{
			// line contains data AND its not comment (cause fuck comment i dont care bout em)
			if (line.length() > 0 && line[0] != '#')
			{
				/* if (regex_search(line, matches, regex_mtllib))
				{
					for (int i = 1; i < matches.size(); i++)
					{
						if (i == 1)
							mtllib_files_paths.push_back(matches[i].str());
					}
				}
				else if (regex_search(line, matches, regex_usemtl))
				{
					for (int i = 1; i < matches.size(); i++)
					{
						if (i == 1)
							usemtl_names.push_back(matches[i].str());
					}
				}
				else if (regex_search(line, matches, regex_object_name))
				{
					for (int i = 1; i < matches.size(); i++)
					{
						if (i == 1)
						{
							last_mesh_object = new MeshObject();
							last_mesh_object->name = matches[i].str();
							mesh_objects_.push_back(last_mesh_object);
						}
					}
				}
				else if (regex_search(line, matches, regex_group_name))
				{
					for (int i = 1; i < matches.size(); i++)
					{
						if (i == 1)
						{
							MeshObjectGroup* mesh_object_group = new MeshObjectGroup();
							mesh_object_group->name = matches[i].str();
							// expecting that before each group will be object name
							last_mesh_object->groups.push_back(mesh_object_group);
						}
					}
				}
				else */

				if (regex_search(line, matches, regex_vertex))
				{
					int vertices_count = 0;

					for (int i = 1; i < matches.size(); i++)
					{

						if (i == 1 || i == 3 || i == 5 || (i == 9 && matches[i].length() > 0))
						{
							vertices.push_back(stof(matches[i].str()));
							vertices_count++;
						}
					}

					data.vertices_count = vertices_count;
				}
				else if (regex_search(line, matches, regex_uv_coordinate))
				{
					int uv_coordinates_count = 0;

					for (int i = 1; i < matches.size(); i++)
					{
						if (i == 1 || (i == 5 || i == 9 && matches[i].length() > 0))
						{
							uv_coordinates.push_back(stof(matches[i].str()));
							uv_coordinates_count++;
						}
					}

					data.uv_coordinates_count = uv_coordinates_count;
				}
				else if (regex_search(line, matches, regex_normal))
				{
					int normals_count = 0;

					for (int i = 1; i < matches.size(); i++)
					{
						if (i == 1 || i == 3 || i == 5)
						{
							normals.push_back(stof(matches[i].str()));
							normals_count++;
						}
					}

					data.normals_count = normals_count;
				}
				/* else if (regex_search(line, matches, regex_space_vertex))
				{
					int space_vertices_offset = 0;

					for (int i = 1; i < matches.size(); i++)
					{
						if (i == 1 || (i == 5 || i == 9 && matches[i].length() > 0))
						{
							space_vertices.push_back(stof(matches[i].str()));
							space_vertices_offset++;
						}
					}

					space_vertices_offset = space_vertices_offset;
				}
				else if (regex_search(line, matches, regex_face_vi))
				{
					for (int i = 1; i < matches.size(); i++)
					{
						vertices_indices.push_back(stoul(matches[i].str()) - 1);
					}
				}
				else if (regex_search(line, matches, regex_face_vi_ui))
				{

					for (int i = 1; i < matches.size(); i++)
					{

						if (i % 2 == 0)
							uv_coordinates_indices.push_back(stoul(matches[i].str()) - 1);
						else
							vertices_indices.push_back(stoul(matches[i].str()) - 1);
					}
				} */
				else if (regex_search(line, matches, regex_face_vi_ui_ni))
				{
					for (int i = 1; i < matches.size(); i++)
					{
						int index = stoul(matches[i].str()) - 1;

						if (i % 3 == 0)
						{
							index *= data.normals_count;

							for (int offset = 0; offset < data.normals_count; offset++)
							{
								data.values.push_back(normals[index + offset]);
							}
						}
						else if ((i + 1) % 3 == 0)
						{
							index *= data.uv_coordinates_count;

							for (int offset = 0; offset < data.uv_coordinates_count; offset++)
							{
								data.values.push_back(uv_coordinates[index + offset]);
							}
						}
						else
						{
							index *= data.vertices_count;

							for (int offset = 0; offset < data.vertices_count; offset++)
							{
								data.values.push_back(vertices[index + offset]);
							}
						}
					}
				}
				/* else if (regex_search(line, matches, regex_face_vi_ni))
				{
					for (int i = 1; i < matches.size(); i++)
					{

						if (i % 2 == 0)
							normals_indices.push_back(stoul(matches[i].str()) - 1);
						else
							vertices_indices.push_back(stoul(matches[i].str()) - 1);
					}
				} */
			}
		}
		return data;
	}
}