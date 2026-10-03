{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    gcc
    bear
    sdl3
  ];
}
