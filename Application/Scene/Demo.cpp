#include "Demo.h"
#include "../Camera/Camera.h"
#include "../Shader/glslShader.h"

#include <imgui.h>

//todo: where do we need to make camera?
void Demo::Init(int width, int height, Camera* cam)
{
	camera = cam;

	glViewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, Z_near_far.x, Z_near_far.y);
	view = camera->GetViewMatrix();

	shdr_file_setup();

	mesh_setup();
	Parse_Section(sections, "../3Dmodels/Section", 6);

	//CenterOBJ.init(meshes[static_cast<int>(MeshType::BUNNY)], shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]);
	//CenterOBJ.set_color({ 0.75f,0.45f,0.3f,1.f });
	CenterOBJ.SetShdr_pgm(shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]);

	//total 6 sections.
	for (auto section : sections)
	{
		for (std::string& filepath : section)
		{
			CenterOBJ.Load_Assimp(filepath);
		}
	}

	CenterOBJ.set_color({ 0.f,0.f,0.f,1.f });
	CenterOBJ.Set_mapping(true);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*  Hidden surface removal */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
}

void Demo::Update(double delta_time)
{
	Update_ImGui();	

	view = camera->GetViewMatrix();
	cam_pos = camera->Position;

	CenterOBJ.Update(view, projection, lightPos, lightColor, cam_pos);
	//CenterOBJ.update(shaders[static_cast<int>(ShdrType::MODEL_PHONG)], view, projection, cam_pos, g, pl, m);
}

void Demo::Render()
{
	CenterOBJ.Draw(show_fnormal, show_vnormal);
}

void Demo::CleanUp()
{
	CenterOBJ.CleanUp();

	// todo: Clean Meshes
	//Scene::CleanUp();
}

void Demo::Update_ImGui()
{
	ImGui::Begin("Scene");
	//if (ImGui::Combo("Mesh", &selected_item, mesh_items, IM_ARRAYSIZE(mesh_items)))
	//{
	//	switch (selected_item)
	//	{
	//	case 0:CenterOBJ.init(meshes[static_cast<int>(MeshType::BUNNY)], shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]); break;
	//	case 1:CenterOBJ.init(meshes[static_cast<int>(MeshType::SPHERE4)], shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]); break;
	//	case 2:CenterOBJ.init(meshes[static_cast<int>(MeshType::CUBE2)], shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]); break;
	//	case 3:CenterOBJ.init(meshes[static_cast<int>(MeshType::SPHERE)], shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]); break;
	//	case 4:CenterOBJ.init(meshes[static_cast<int>(MeshType::SPHERE_MOD)], shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]); break;

	//	default: CenterOBJ.init(meshes[static_cast<int>(MeshType::BUNNY)], shdr_files[static_cast<int>(ShdrType::MODEL_PHONG)]); break;
	//	}
	//}

	ImGui::Checkbox("Show face normal ", &show_fnormal);
	ImGui::Checkbox("Show vertex normal", &show_vnormal);

	ImGui::SliderFloat3("Light Position", &lightPos.x, -1000.f, 1000.f);
	ImGui::End();
}

//todo: ugly. maybe we need this func in Scene with filepath, size params
//void Demo::Parse_Section()
//{
//	std::string section = "../3Dmodels/Section";
//	sections.resize(6);
//
//	//pushback sections[0]~[5]
//	for (int i{ 0 }; i < 6; i++)
//	{
//		std::ifstream inFile(section + std::to_string(i + 1) + ".txt");
//		if (inFile.is_open() == false)
//		{
//			//throw std::runtime_error("Failed to load Section" + std::to_string(i));
//			continue;
//		}
//
//		std::string label;
//		while (inFile.eof() == false)
//		{
//			getline(inFile, label);
//			if (label.empty() || std::string(label.end() - 4, label.end()) != ".obj")
//			{
//				//throw std::runtime_error("Bad Filetype.  " + label + " not a .obj file");
//				continue;
//			}
//
//			sections[i].push_back(label);
//		}
//	}
//}

void Demo::mesh_setup()
{
	std::string sphere4{ "../3Dmodels/4Sphere.obj" };
	std::string bunny{ "../3Dmodels/bunny_high_poly.obj" };
	std::string cube2{ "../3Dmodels/cube2.obj" };
	std::string sphere{ "../3Dmodels/sphere.obj" };
	std::string sphere_mod{ "../3Dmodels/sphere_modified.obj" };
	//std::string Cen_path{"../3Dmodels/cup.obj"};				\par?
	//std::string Cen_path{"../3Dmodels/lucy_princeton.obj"};	?
	//std::string Cen_path{"../3Dmodels/starwars1.obj"};		many obj in one file


	/* Procedrual Meshes. Maybe make each Model and assign these meshes.
	meshes.push_back(new Mesh(bunny));
	meshes.push_back(new Mesh(sphere4));
	meshes.push_back(new Mesh(cube2));
	meshes.push_back(new Mesh(sphere));
	meshes.push_back(new Mesh(sphere_mod));
	*/
}

//scene으로 넘겨? 그럼 vector<pair<string,string>>으로?
void Demo::shdr_file_setup()
{
	std::vector<shdr_vec> v;

	shdr_vec model_shdr_files;
	model_shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/model_shader.vert"));
	model_shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/model_shader.frag"));
	v.push_back(model_shdr_files);

	shdr_vec line_shdr_files;
	line_shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/line_shader.vert"));
	line_shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/line_shader.frag"));
	v.push_back(line_shdr_files);

	Load_Shaders(v);
}

//======================================================================================================================================

//Mesh* CreateSphere(int stacks, int slices)
//{
//	Mesh* mesh = new Mesh();
//
//	for (int i = 0; i <= stacks; i++)
//	{
//		float row = static_cast<float>(i) / stacks;
//		float beta = glm::pi<float>() * (row - 0.5f);
//		for (int j = 0; j <= slices; j++)
//		{
//			Vertex vertex;
//			float col = static_cast<float>(j) / slices;
//			vertex.uv.x = -col;
//			vertex.uv.y = -row;
//
//			float alpha = col * glm::pi<float>() * 2.0f;
//			vertex.pos.x = sin(alpha) * cos(beta);
//			vertex.pos.y = sin(beta);
//			vertex.pos.z = cos(alpha) * cos(beta);
//
//			vertex.nrm.x = vertex.pos.x;
//			vertex.nrm.y = vertex.pos.y;
//			vertex.nrm.z = vertex.pos.z;
//			//vertex.nrm /= radius;
//
//			mesh->vertexBuffer.push_back(vertex);
//		}
//	}
//	mesh->BuildIndexBuffer(stacks, slices);
//	mesh->calc_BufferDatas();
//	mesh->SendVertexData();
//
//	return mesh;
//}
//
//Mesh* CreateOrbit(float radius, GLuint vert)
//{
//	Mesh* mesh = new Mesh();
//	float angle = 360.f / vert;
//
//	mesh->lineposBuffer.push_back(glm::vec3(radius * glm::cos(0.f), 0.f, radius * glm::sin(0.f)));
//
//	for (GLuint i{ 1 }; i < vert; i++)
//	{
//		glm::vec3 pos = glm::vec3(radius * glm::cos(glm::radians(angle * i)), 0.f, radius * glm::sin(glm::radians(angle * i)));
//		mesh->lineposBuffer.push_back(pos);
//		mesh->lineposBuffer.push_back(pos);
//	}
//
//	mesh->lineposBuffer.push_back(mesh->lineposBuffer[0]);
//	mesh->LineVertexData();
//
//	return mesh;
//}