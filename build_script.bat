mkdir obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/render_constants.ixx /Foobj/render_constants.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/custom_concepts.ixx /Foobj/custom_concepts.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/vec.ixx /Foobj/vec.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/matrix.ixx /Foobj/matrix.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/light.ixx /Foobj/light.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/material.ixx /Foobj/material.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/ray.ixx /Foobj/ray.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/polygon.ixx /Foobj/polygon.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/surface.ixx /Foobj/surface.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/shell.ixx /Foobj/shell.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/camera.ixx /Foobj/camera.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/saveppm.ixx /Foobj/saveppm.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/renderer.ixx /Foobj/renderer.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD src/test.ixx /Foobj/test.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD application.ixx /Foobj/application.obj 
cl.exe obj/application.obj obj/render_constants.obj obj/custom_concepts.obj obj/vec.obj obj/matrix.obj obj/light.obj obj/material.obj obj/ray.obj obj/polygon.obj obj/surface.obj obj/shell.obj obj/camera.obj obj/saveppm.obj obj/renderer.obj obj/test.obj 
