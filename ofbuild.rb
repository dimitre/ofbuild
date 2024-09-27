class Ofbuild < Formula
  desc "Simple command-line tool for sending OSC packet"
  homepage "https://github.com/dimitre/ofbuild"
  sha256 "fd3a693ed5c2b5a1de128369b4bf1545680ad460575d2a292f74cb60b757f50d"
  url "https://github.com/dimitre/ofbuild/archive/refs/tags/v0.0.6.tar.gz"
  sha256 "fd3a693ed5c2b5a1de128369b4bf1545680ad460575d2a292f74cb60b757f50d"
  depends_on "yaml-cpp"
  depends_on "fmt"

  def install
    system "g++", 
          "-std=c++17",
          "-I#{Formula["yaml-cpp"].opt_include}",
          "-I#{Formula["fmt"].opt_include}",
          "-L#{Formula["yaml-cpp"].opt_lib}",
          "-L#{Formula["fmt"].opt_lib}",
          "-lfmt",
          "-lyaml-cpp",
          "src/main.cpp",
          "-o",
          "#{prefix}/ofbuild",
          "-O3",
  end
end  

# __END__