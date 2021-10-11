# Ray tracer
Interesting project, as it has been quite interesting to learn a lot about what to do and what not to do when writing a ray tracer.
As this was one of my first ray tracers, it taught me so much about what goes into making a working ray tracer.
I am not entirely happy with how this one turned out, so I am planning on writing a new one where I will apply everything I learnt from this project :)

# What the ray tracer can do
- Sphere & Plane shape intersection
- Triangle mesh intersection
- Material rendering: Lambda, Phong & Cook-Torrance
- Cull modes for meshes: Front-Face Culling, Back-Face Culling & No Culling
- Point Light & Directional Light
- Camera movement (controlled keyboar & mouse, can be controlled entirely with keyboard)
- Hard shadows
- Cycelable Scenes

# Photos
In this scene from left to right there are 
- 3 Triangles
	rendered with: Back-Face Culling, Front-Face Culling & No Culling.
- 6 Spheres
	Top 3 are rendered with metallic cook-torrance materials: Rough Silver, Mat Gold & Shiny Zinc (roughness 1, 0.6, 0.1)
	bottom 3 rendered with dilectric cook-torrance materials: Rough-Green, Mat-Blue, Shiny-Red
- 4 Lights
 3 Pointlights & 1 Directional light
![Raytracer_scene_Stanford_Bunny](https://raw.githubusercontent.com/PrebenRonningen/RayTracer/main/Photos/Bunny.bmp)

In this scene 
- 1 Stanford Bunny with a White Lambert material
- 3 Point Lights
- 1 Directional Light
![Raytracer_scene_Stanford_Bunny](https://raw.githubusercontent.com/PrebenRonningen/RayTracer/main/Photos/Bunny.bmp)