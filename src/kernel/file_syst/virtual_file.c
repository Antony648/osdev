#include "./virtual_file"
#include "../string/string.h"
#include "../ctype/ctype.h"
//implement core vfs 

struct mount_point_ent* get_mount_point(const char* path)
{
	struct mount_point_ent* rtn_val;
	return rtn_val;	
}
struct file_desc* vfs_open(const char* cur_dir,const char* path)
{
	
	char* ppath;
	if(isdigit(path[0]))		//contains full path with disk id can work with, not general
		ppath=(char*)path;
	else
	{
		uint32_t len1=strlen(cur_dir);
		uint32_t len=strlen(path);

	}

}