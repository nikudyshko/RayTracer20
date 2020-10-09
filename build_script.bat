mkdir obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/render_constants.ixx /Foobj/render_constants.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/custom_concepts.ixx /Foobj/custom_concepts.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/vec.ixx /Foobj/vec.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/matrix.ixx /Foobj/matrix.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/light.ixx /Foobj/light.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/material.ixx /Foobj/material.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/ray.ixx /Foobj/ray.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/hitnode.ixx /Foobj/hitnode.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/geometry.ixx /Foobj/geometry.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/polygon.ixx /Foobj/polygon.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/sphere.ixx /Foobj/sphere.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/surface.ixx /Foobj/surface.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/shell.ixx /Foobj/shell.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/camera.ixx /Foobj/camera.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/saveppm.ixx /Foobj/saveppm.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/renderer.ixx /Foobj/renderer.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 src/test.ixx /Foobj/test.obj 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD /O2 application.ixx /Foobj/application.obj 
cl.exe obj/application.obj obj/render_constants.obj obj/custom_concepts.obj obj/vec.obj obj/matrix.obj obj/light.obj obj/material.obj obj/ray.obj obj/hitnode.obj obj/geometry.obj obj/polygon.obj obj/sphere.obj obj/surface.obj obj/shell.obj obj/camera.obj obj/saveppm.obj obj/renderer.obj obj/test.obj 
