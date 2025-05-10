{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    cmake
    ninja
    sdl3
  ];
  shellHook = ''
    
  '';
}
