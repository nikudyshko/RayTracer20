cl.exe /std:c++latest /experimental:module /c /EHsc /MD vec.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD saveppm.ixx 
cl.exe /std:c++latest /experimental:module /c /EHsc /MD application.ixx 
cl.exe application.obj vec.obj saveppm.obj 
