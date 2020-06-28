cl.exe /std:c++latest /experimental:module /c /EHsc /MD render_constants.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD custom_concepts.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD vec.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD ray.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD material.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD polygon.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD surface.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD shell.ixx  
cl.exe /std:c++latest /experimental:module /c /EHsc /MD camera.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD saveppm.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD application.ixx 
cl.exe application.obj render_constants.obj custom_concepts.obj vec.obj ray.obj material.obj polygon.obj surface.obj shell.obj camera.obj saveppm.obj 
