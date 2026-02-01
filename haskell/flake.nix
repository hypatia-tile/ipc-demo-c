{
  description = "Instant Haskell dev env";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };

        # Pick a compiler once. Change if you want.
        hp = pkgs.haskell.packages.ghc910; # e.g. ghc96, ghc98, ghc910, etc.

        commonTools = [
          hp.ghc
          hp.cabal-install
          hp.hoogle
          hp.haskell-language-server
          hp.ghcid
          hp.ormolu
          hp.hlint

          # Helpful general tools
          pkgs.gnumake

          # Add frequently used packages here
          hp.bytestring
          hp.text
          hp.containers
          hp.network
          hp.directory
          hp.unix
        ];
      in
      {
        devShells = {
          research = pkgs.mkShell {
            name = "hs-research";
            buildInputs = [ pkgs.nil ];
            packages = commonTools ++ [
              pkgs.time
              pkgs.hyperfine
              pkgs.which
            ];

            shellHook = ''
              echo "Haskell research shell: ghc=$(ghc --numeric-version), cabal=$(cabal --numeric-version)"
              echo "Try:  cabal repl  |  ghci  |  ghcid --command 'cabal repl'"
            '';
          };

          default = self.devShells.${system}.research;
        };
      }
    );
}
