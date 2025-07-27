//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */

#include "pk2_lua.hpp"
#include <string>

#include "lua_level.hpp"
#include "lua_sprite_class.hpp"
#include "lua_sprites_list.hpp"

#include "lua_game_events.hpp"
#include "lua_sprite_commands.hpp"
#include "lua_misc.hpp"

#include "life.hpp"

#include "engine/PLog.hpp"
#include "engine/PFile.hpp"
#include "engine/PFilesystem.hpp"

#include "game/game.hpp"

#include "system.hpp"

namespace PK2lua{

std::string PK2GetLuaFile(const std::string&filename_in){
    std::string name = filename_in;
    if(name.size() > 4 && name.substr(name.size()-4, 4)!=".lua"){
        name += ".lua";
    }

    std::optional<PFile::Path> file = PFilesystem::FindAsset(name, PFilesystem::LUA_DIR);

    if(file.has_value()){
        return file->GetContentAsString();
    }
    else{
        return "";
    }    
}

/**
 * @brief 
 * Override lua require to enable loading modules from a zipped episode
 */

void OverrideLuaRequire(sol::state& lua){

    lua["_pk2_get_lua_file"] = PK2GetLuaFile;

    sol::state* lua_ptr = &lua;

    lua["_pk2_load_string"] = [lua_ptr](const std::string&lua_code){
        return lua_ptr->safe_script(lua_code);
    };

    static const char * require_decorator = 
        "local _require = require \n"
        "require = function(mod_name) \n"
            "if mod_name == \"pk2\" then \n"
                "return _pk2_api \n"
            "else \n"
                "tmp = _pk2_get_lua_file(mod_name) \n"
                "if tmp ~= \"\" then \n"
                    "return _pk2_load_string(tmp) \n"
                "elseif _require ~= nil then \n"
                    "return _require(mod_name) \n"
                "end \n"
            "end \n"
        "end \n";

    lua.safe_script(require_decorator);
}

sol::state* CreateGameLuaVM(const std::string& main_lua_script){
    if(main_lua_script.empty()){
        PLog::Write(PLog::INFO, "PK2lua", "No Lua scripting in this level");
        return nullptr;
    }

    std::string main_lua = PK2GetLuaFile(main_lua_script);
    if(main_lua.empty()){
        throw std::runtime_error("Lua script: \""+main_lua_script+"\" not found!");
    }
    
    PLog::Write(PLog::INFO, "PK2lua", "Creating lua VM");
    sol::state* lua = new sol::state();
    lua->open_libraries(sol::lib::base,
            sol::lib::string,
            sol::lib::table,
            sol::lib::math,
            sol::lib::coroutine,
            sol::lib::utf8,
            sol::lib::package);

    OverrideLuaRequire(*lua);

    PLog::Write(PLog::INFO, "PK2lua", "Running main.lua");

    sol::table PK2_API = lua->create_table();

    ExposeSectorClass(*lua, PK2_API);
    ExposePrototypeClass(*lua);
    ExposeSpriteClass(*lua);

    

    ExposeSpriteListAPI(PK2_API);
    ExposeCommandsAPI(PK2_API);
    ExposeEventsAPI(*lua, PK2_API);
    ExposeMiscAPI(PK2_API);

    ExposeGameOfLife(PK2_API);


    (*lua)["_pk2_api"] = PK2_API;

    lua->safe_script(main_lua);
    return lua;
}

void UpdateLua(){
    UpdateGameOfLife();
    TriggerEventListeners(LUA_EVENT_GAME_TICK);
}

void DestroyGameLuaVM(sol::state *& lua){
    ClearEvents();
    ClearCommands();
    ClearGameOfLife();
    delete lua;
    lua = nullptr;
}

}