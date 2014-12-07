/*----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#------  This File is Part Of : ----------------------------------------------------------------------------------------#
#------- _  -------------------  ______   _   --------------------------------------------------------------------------#
#------ | | ------------------- (_____ \ | |  --------------------------------------------------------------------------#
#------ | | ---  _   _   ____    _____) )| |  ____  _   _   ____   ____   ----------------------------------------------#
#------ | | --- | | | | / _  |  |  ____/ | | / _  || | | | / _  ) / ___)  ----------------------------------------------#
#------ | |_____| |_| |( ( | |  | |      | |( ( | || |_| |( (/ / | |  --------------------------------------------------#
#------ |_______)\____| \_||_|  |_|      |_| \_||_| \__  | \____)|_|  --------------------------------------------------#
#------------------------------------------------- (____/  -------------------------------------------------------------#
#------------------------   ______   _   -------------------------------------------------------------------------------#
#------------------------  (_____ \ | |  -------------------------------------------------------------------------------#
#------------------------   _____) )| | _   _   ___   ------------------------------------------------------------------#
#------------------------  |  ____/ | || | | | /___)  ------------------------------------------------------------------#
#------------------------  | |      | || |_| ||___ |  ------------------------------------------------------------------#
#------------------------  |_|      |_| \____|(___/   ------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Licensed under the GPL License --------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Copyright (c) Nanni <lpp.nanni@gmail.com> ---------------------------------------------------------------------------#
#- Copyright (c) Rinnegatamante <rinnegatamante@gmail.com> -------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Credits : -----------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Smealum for ctrulib -------------------------------------------------------------------------------------------------#
#- Aurelio for testing & bug-fixing ------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <3ds.h>
#include "include/luaplayer.h"
#include "include/luaGraphics.h"

#define stringify(str) #str
#define VariableRegister(lua, value) do { lua_pushinteger(lua, value); lua_setglobal (lua, stringify(value)); } while(0)

static int lua_print(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 5) return luaL_error(L, "wrong number of arguments");
	int x = luaL_checkint(L, 1);
    int y = luaL_checkint(L, 2);
	char* text = (char*)(luaL_checkstring(L, 3));
	u32 color = luaL_checknumber(L,4);
	int screen = luaL_checknumber(L,5);
	DrawText(x,y,text,color,screen);
	return 0;
}

static int lua_bitmap(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return luaL_error(L, "wrong number of arguments");
	char* text = (char*)(luaL_checkstring(L, 1));
	Bitmap file = LoadBitmap(text);
	int i=1;
	lua_newtable(L);
	lua_pushstring(L, "offset");
    lua_pushnumber(L, (u32)(file.pixels));
    lua_settable(L, -3);
	lua_pushstring(L, "width");
    lua_pushnumber(L, file.width);
    lua_settable(L, -3);
	lua_pushstring(L, "height");
    lua_pushnumber(L, file.height);
    lua_settable(L, -3);
	return 1;
}

static int lua_pbitmap(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 5) return luaL_error(L, "wrong number of arguments");
	int i=1;
	Bitmap file;
	int x = luaL_checkint(L, 1);
    int y = luaL_checkint(L, 2);
	file.pixels = (u8*)luaL_checkint(L, 3);
	file.width = luaL_checkint(L, 4);
	file.height= luaL_checkint(L, 5);
	int screen= luaL_checkint(L, 6);
	PrintBitmap(x,y,file,screen);
	return 0;
}

static int lua_flip(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments");
	gfxFlushBuffers();
	gfxSwapBuffers();
	return 0;
}

static int lua_refresh(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments");
	RefreshScreen();	
	return 0;
}

static int lua_Vblank(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments");
	gspWaitForVBlank();
	return 0;
}

static int lua_clearScreen(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return luaL_error(L, "wrong number of arguments");
	int screen = luaL_checkint(L,1);
	ClearScreen(screen);
	return 0;
}

static int lua_fillRect(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 6) return luaL_error(L, "wrong number of arguments");
	int x1 = luaL_checkint(L,1);
	int x2 = luaL_checkint(L,2);
	int y1 = luaL_checkint(L,3);
	int y2 = luaL_checkint(L,4);
	u32 color = luaL_checknumber(L,5);
	int screen = luaL_checkint(L,6);
	FillRect(x1,x2,y1,y2,color,screen);
	return 0;
}

static int lua_fillEmptyRect(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 6) return luaL_error(L, "wrong number of arguments");
	int x1 = luaL_checkint(L,1);
	int x2 = luaL_checkint(L,2);
	int y1 = luaL_checkint(L,3);
	int y2 = luaL_checkint(L,4);
	u32 color = luaL_checknumber(L,5);
	int screen = luaL_checkint(L,6);
	FillEmptyRect(x1,x2,y1,y2,color,screen);
	return 0;
}

static int lua_pixel(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 4) return luaL_error(L, "wrong number of arguments");
	int x = luaL_checknumber(L,1);
	int y = luaL_checknumber(L,2);
	u32 color = luaL_checknumber(L,3);
	int screen = luaL_checknumber(L,4);
	DrawPixel(x,y,color,screen);
	return 0;
}

static int lua_color(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 3) return luaL_error(L, "wrong number of arguments");
    int r = luaL_checkint(L, 1);
    int g = luaL_checkint(L, 2);
	int b = luaL_checkint(L, 3);
    u32 color = b | (g << 8) | (r << 16);
    lua_pushnumber(L,color);
    return 1;
}

static int lua_getB(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 1) return luaL_error(L, "wrong number of arguments");
    int color = luaL_checkint(L, 1);
    u32 colour = color & 0xFF;
    lua_pushnumber(L,colour);
    return 1;
}

static int lua_getG(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 1) return luaL_error(L, "wrong number of arguments");
    int color = luaL_checkint(L, 1);
    u32 colour = (color >> 8) & 0xFF;
    lua_pushnumber(L,colour);
    return 1;
}

static int lua_getR(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 1) return luaL_error(L, "wrong number of arguments");
    int color = luaL_checkint(L, 1);
    u32 colour = (color >> 16) & 0xFF;
    lua_pushnumber(L,colour);
    return 1;
}


//Register our Color Functions
static const luaL_Reg Color_functions[] = {
  {"new",                				lua_color},
  {"getR",								lua_getR},
  {"getG",								lua_getG},
  {"getB",								lua_getB},
  {0, 0}
};

//Register our Screen Functions
static const luaL_Reg Screen_functions[] = {
  {"debugPrint",					lua_print},
  {"waitVblankStart",				lua_Vblank},
  {"flip",							lua_flip},
  {"refresh",						lua_refresh},
  {"clear",							lua_clearScreen},
  {"fillRect",						lua_fillRect},
  {"fillEmptyRect",					lua_fillEmptyRect},
  {"pixel",							lua_pixel},
  {"loadBitmap",					lua_bitmap},
  {"lpp_secret_0452",				lua_pbitmap},
  {0, 0}
};

void luaScreen_init(lua_State *L) {
	lua_newtable(L);
	luaL_setfuncs(L, Screen_functions, 0);
	lua_setglobal(L, "Screen");
	lua_newtable(L);
	luaL_setfuncs(L, Color_functions, 0);
	lua_setglobal(L, "Color");
	int TOP_SCREEN = 0;
	int BOTTOM_SCREEN = 1;
	VariableRegister(L,TOP_SCREEN);
	VariableRegister(L,BOTTOM_SCREEN);
}