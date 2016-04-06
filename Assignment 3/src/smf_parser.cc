//////////////////////////////////////////////////////////////////////////////
//
//  smf_parser.cc
//
//  The source file for function that import/export mesh data file
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 2/11/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "smf_parser.h"

extern raychess::Object *scene;

namespace smfparser {

// The mesh vertices stored in memory
vector<glm::vec3> mesh_vertex;

//
// Function: Split
// ---------------------------
//
//   A template function for string split
//
//   Parameters:
//       T: the class type
//       s: the string that needs to split
//       delim: the delaminator that used to split s
//       ret: the returned vector made by splited elements
//
//   Returns:
//       void
//

template <class T>
void split(string &s, char delim, vector<T> &ret) {
    stringstream ss(s);
    string str;
    while (getline(ss, str, delim)) {
        if (str.compare("") != 0)
            ret.push_back((T)stof(str));
    }
}

//
// Function: TrimLeadingSpace
// ---------------------------
//
//   Trim the leading space of a string
//
//   Parameters:
//       s: the string that needs to trim
//
//   Returns:
//       void
//

void TrimLeadingSpace(string &s) {
    size_t pos = s.find_first_not_of(' ');
    if (pos != string::npos && pos != 0) {
        s.erase(0, pos - 1);
    }
}

//
// Function: TrimTailingSpace
// ---------------------------
//
//   Trim the tailing space of a string
//
//   Parameters:
//       s: the string that needs to trim
//
//   Returns:
//       void
//

void TrimTailingSpace(string &s) {
    size_t pos = s.find_last_not_of(" \r\n");
    if (pos != string::npos) {
        s.erase(pos + 1);
    }
}

//
// Function: ImportMeshFile
// ---------------------------
//
//   Import mesh data from file to memory
//
//   Parameters:
//       file_path: the path of the import file
//       scale: the scale factor of the imported mesh
//       rotate: the rotate factor of the imported mesh
//       offset: the offset of the imported mesh
//       index: the index of the object in the scene
//       color: the color of the imported mesh
//
//   Returns:
//       void
//

void ImportMeshFile(string file_path, float scale, float rotate, glm::vec3 offset, int &index, glm::vec3 color) {

    string line;
    ifstream fin("chess_pieces/" + file_path, ifstream::in);

    if (!fin.is_open()) {       // Check if input file exist
        cout << "Mesh file doesn't exist!" << endl;
        return;
    }

    int flag = libconsts::kFlagVertex;      // Init flag and clear mesh data
    mesh_vertex.clear();

    while (!fin.eof()) {        // While not end of file
        getline(fin, line);
        if (line.find_first_not_of(' ') == string::npos) continue;    // If is blank line
        TrimLeadingSpace(line);                 // Trim leading space
        if (line[0] == '#') continue;           // If is comment
        if (line[0] == 'f') flag = libconsts::kFlagFace;
        line.erase(0, 2);
        TrimTailingSpace(line);                 // Trim tailing space
        if (flag == libconsts::kFlagVertex) {       // If this line contains vertex data
            vector<float> pos;
            split(line, ' ', pos);
            if (pos.size() >= 3) {
                glm::vec3 new_vertex = glm::vec3(pos[0], pos[1], pos[2]);
                new_vertex = new_vertex * scale;                // Scale mesh vertices
                new_vertex = glm::rotateZ(new_vertex, rotate * libconsts::kDegreeToRadians);    // Rotate mesh vertices
                new_vertex = new_vertex + offset;               // Transform mesh vertices
                mesh_vertex.push_back(new_vertex);              // Add new vertex
            } else {                                // If size is less than 3, then this is not a valid smf file
                cout << "Fatal! Smf file import error!" << endl;
                exit(EXIT_FAILURE);
            }
        } else {
            vector<int> face_vertex;
            split(line, ' ', face_vertex);
            int size = (int)face_vertex.size();
            if (size >= 3) {                        // Mesh with many triangles
                glm::vec3 v1, v2, v3;
                bool infinite = false;
                float triangle_shininess = libconsts::kMeshShininess;
                float triangle_reflectance = libconsts::kMeshReflectance;
                float triangle_refractance = libconsts::kMeshRefractance;
                float triangle_reflect_ratio = libconsts::kMeshRefractRatio;
                v1 = mesh_vertex[face_vertex[0] - 1];
                v2 = mesh_vertex[face_vertex[1] - 1];
                v3 = mesh_vertex[face_vertex[2] - 1];
                scene = AddTriangle(scene, v1, v2, v3, color, color, color, triangle_shininess, triangle_reflectance,
                                    triangle_refractance, triangle_reflect_ratio, ++index, infinite);
            } else {                                // If size is less than 3, then this is not a valid smf file
                cout << "Fatal! Smf file import error!" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    fin.close();
    cout << "Import Finished" << endl;
}

}  // namespace smfparser
