with import <nixpkgs> {};
stdenv.mkDerivation rec {
  name = "env";
  env = buildEnv { name = name; paths = buildInputs; };
  buildInputs = [
     clang 
     glew 
     glm 
     glfw 
     mesa 
     xorg.libX11 
     xorg.libXrandr 
     xorg.libXinerama 
     xorg.libXi 
     xorg.libXcursor  
  ];
}
