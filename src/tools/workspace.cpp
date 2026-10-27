// src/tools/workspace.cpp — YOU IMPLEMENT (spec §4.4).
//
// Workspace::resolve: a purely lexical check, steps 1–6 of the spec.
// Do not touch the filesystem here (no exists(), canonical(), ...).
// The constructor, root() and check_symlinks() are provided in
// workspace_provided.cpp.

#include "tools/workspace.h"

namespace fs = std::filesystem;

bool Workspace::resolve(std::string_view user_path, fs::path* out,std::string* reason) const {

    if (user_path == ""){
        *reason = "empty path";
    }
    else if (user_path.find('\0') != std::string_view::npos){
        *reason = "NUL";
    }
    else{
        fs::path p {std::string(user_path)};
        if (p.is_absolute() || p.has_root_path()){
            *reason = "absolute";
            return false;
        }
        else{
            fs::path n = p.lexically_normal();

            if (n.begin() != n.end() && *n.begin() == ".."){
                *reason = "no working directory";
                return false;
            }
            else{
                if (n == "."){
                    *out = root();
                    
                }
                else{
                    *out = root() / n;
                }

                return true;
            }
        }
    }

    return false;
}
