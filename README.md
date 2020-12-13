# opengl-obj-loader
Simple opengl .obj loader

## Usage
```
#include "OBJLoader.h"

int maint()
{
  OBJLoader::OBJData data = OBJLoader::LoadOBJ("special/path/to/your/beautiful/file.obj");
}
```

## Description

- `OBJData`
  - ```vector<float> data;``` - contains data for opengl buffer ( vx-vy-vz-uvx-uvy-nx-ny-nz... )
  - ```int vertices_count;``` - how many axis are specified (3: x, y, z, 4: x, y, z, w)
  - ```int normals_count;``` - how many axis are specified (3: x, y, z)
  - ```int uv_coordinates_count;``` - how many axis are specified (2: u, v, 3: u, v, w)
  
- because of how .obj file is constructed (order), offsets for vertices, normals and uv coordinates are strictly specified depending on their count
- the order is: vertex, uv coordinate, normal
