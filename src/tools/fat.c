/*
not all have faith.But the Lord is faithful,and he will strengthen you and
keep you from the evil one 
    -2Thessanlonians 3:3 
*/

//there is a small issue with the code like,not an actual issue ,more of an unchecked condition
//the readsector, reads by sectors, if your file is less than a cluster,then the readsector will try 
//to write contents to your file buffer but if your file buffer is of size of the actual target file 
// then the it will be less than a cluster this extra data will be written anyways resulting in 
// heap overflow (assuming that buffer is dynamically allocated).
// i fixed it :)

#include <ctype.h>
#include<stdbool.h>
#include <stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <string.h>
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t doubleword;
typedef struct
{
    byte    jump_state[3];
    byte    bdb_oem[8];         //dos version
    word    bdb_bytes_per_sector;   
    byte    bdb_sector_per_cluster;
    word    bdb_reserved_sector;  //no. of reserved sectors
    byte    bdb_fat_count;  //no. of fat tables
    word    bdb_root_entries;    //no .of dir entries inroot
    word    bdb_number_sectors; //total no. of logical sectors in volume
    byte    bdb_media_desc;    
    word    bdb_sector_per_fat;
    word    bdb_sector_per_track;
    word    bdb_heads;
    doubleword    bdb_hidden_sect;
    doubleword    bdb_large_sect_count;
    //extended boot record
   
    byte    ebr_drive_num;
    byte    ebr_flag;
    byte    ebr_signature;
    byte    ebr_volume_id[4];
    byte    ebr_volume_label[11];
    byte    ebr_file_sys_indentify[8];
} __attribute__((packed)) bios_header_t;


typedef struct 
{
    /* data */
    byte    name[11];
    byte    attr;
    byte    reserved;
    byte    time_creat_sec;
    word    time_creat;
    word    date_creat;
    word    date_last;
    word    cluster_no_high;
    word    time_mod;
    word    date_mod;
    word    cluster_no_low;
    doubleword  size;   //size in bytes
}__attribute__((packed)) fat12_dir_t;


bios_header_t g_boot_header;
uint8_t * g_fat;
fat12_dir_t * root_dir_entry;
uint32_t    dir_size;

bool read_boot_header(FILE*file)
{
   return fread(&g_boot_header,sizeof(bios_header_t),1,file);
}
//this function reads values of a single sector and stores it in buffer at
//param1,
//starting from sector number specified in lba param2,
//number of sectors to be read int uint32_t
//file to be read from
bool readSector(void* out_buf,uint32_t lba,uint32_t count,FILE* file)
{
    return((fseek(file,lba*g_boot_header.bdb_bytes_per_sector,SEEK_SET)==0)&&
    (fread(out_buf,g_boot_header.bdb_bytes_per_sector,count,file))==count);
}
//it loads the content of fat to the g_fat
//a fat12 cluster is 12bits but we use 8bits 
//thats why we have to take extrasteps in retriving
//next cluster
bool readFat(FILE* file)
{
    uint32_t lba=g_boot_header.bdb_reserved_sector;
    uint32_t count=g_boot_header.bdb_fat_count*g_boot_header.bdb_sector_per_fat;
    g_fat=(uint8_t*)malloc(count*g_boot_header.bdb_bytes_per_sector);
    if(!g_fat)
        return false;
    
    return readSector(g_fat,lba,count,file);
    
}
bool readRootDir(FILE * file)
{
    uint32_t lba=g_boot_header.bdb_reserved_sector+
    (g_boot_header.bdb_fat_count*g_boot_header.bdb_sector_per_fat);

    root_dir_entry=(fat12_dir_t*)malloc(g_boot_header.bdb_root_entries*sizeof(fat12_dir_t));
    if(!root_dir_entry)
        return false;

    dir_size=(sizeof(fat12_dir_t)*g_boot_header.bdb_root_entries+g_boot_header.bdb_bytes_per_sector-1)/g_boot_header.bdb_bytes_per_sector;
    
    return readSector(root_dir_entry,lba,dir_size,file);

}
bool search_root(char* file_name,uint32_t* index,FILE* file)
{
    for(uint32_t i=0;i<g_boot_header.bdb_root_entries;i++)
    {
        if(!root_dir_entry[i].name[0])
            break;
        if(memcmp(file_name,root_dir_entry[i].name,11)==0)
            {
      //          printf("found %s among directory entry\n",root_dir_entry[i].name);
                *index=i;
                return true;
            }
    }
    return false;
}
word next_cluster(word cur_clust)
{
    //printf("hola\n");
    //printf("cur_clust:%d\n",cur_clust);
    word off=cur_clust*1.5;
    
    word rtn_val;
    if(cur_clust%2)
        rtn_val= (word)(g_fat[off+1]<<4)|(word)((g_fat[off]&0xF0)>>4);
    else
        rtn_val= (word)(g_fat[off])|(word)((g_fat[off+1]& 0x0F)<<8);
    
    return rtn_val;
}
bool read_file(char* file_name,FILE* file)
{
    uint32_t index;
    if(!search_root(file_name,&index,file))
        return false;
    uint16_t curr_cluster=root_dir_entry[index].cluster_no_low;

    //byte* buf_out=(byte*)malloc(root_dir_entry[index].size);

    uint32_t cluster_bytes=g_boot_header.bdb_sector_per_cluster*g_boot_header.bdb_bytes_per_sector;
    uint32_t addr=g_boot_header.bdb_reserved_sector+(g_boot_header.bdb_fat_count *g_boot_header.bdb_sector_per_fat)
    +dir_size;
    uint32_t lba;
    byte* buf_out=(byte*)malloc(cluster_bytes*(int)((root_dir_entry[index].size+cluster_bytes-1)/cluster_bytes));
    //ensures that the size of buf_out is a multiple of cluster_bytes                              
    
    if(buf_out==NULL)
        return false;
    
    byte* start_loc=buf_out;

    do
    {   
        lba=addr+((curr_cluster-2)*g_boot_header.bdb_sector_per_cluster);
        //error originates here
        if(!readSector(start_loc,lba,g_boot_header.bdb_sector_per_cluster,file))
            return false;
        start_loc+=cluster_bytes;
        
        curr_cluster=next_cluster(curr_cluster);
        //printf("cluster value a %d",curr_cluster);
        //curr_cluster=(word)0xfff;
        
    } while (curr_cluster< 0xff8);
    

    for(doubleword i=0;i<root_dir_entry[index].size;i++)
    {
        
        if(isprint(buf_out[i]))
            fprintf(stderr,"%c",buf_out[i]);
        else
            printf("<%02x>",buf_out[i]);
    }    
    free(buf_out);
    return true;
    
}


int main(int argc,char** argv)
{
    if(argc<3)
    {
        printf("usage %s <filename> <targetfile>",argv[0]);
        return -1;
    }
    FILE *disk=fopen(argv[1],"rb");
    if(!disk)
    {
        fprintf(stderr,"failed to open disk\n");
        return -2;
    }
    //printf("disk opening successfull.\n");
    if(!read_boot_header(disk))
    {
        fprintf(stderr,"failed to read boot header\n");
        return -3;
    }
    //printf("read boot header.\n");
    if(!readFat(disk))
    {
        fprintf(stderr,"failed to read fat tables\n");
        free(g_fat);
        return -4;
    }
    //printf("read fat tables\n");
    if(!readRootDir(disk))
    {
        fprintf(stderr,"failed to read from root\n");
        free(g_fat);
        free(root_dir_entry);        
        return -5;  
    }
    // printf("read root successfully.\n");
    uint32_t file_entry_index;
    if(!search_root(argv[2],&file_entry_index,disk))
    {
        fprintf(stderr,"could not find %s from root\n",argv[2]);
        free(g_fat);
        free(root_dir_entry);        
        return -6;   
    }
    // printf("found %s root successfully.\n",argv[2]);
    if(!read_file(argv[2],disk))
    {
        fprintf(stderr,"failed to read file.\n");
        free(g_fat);
        free(root_dir_entry);
        return -7;
    }
    free(g_fat);
    //printf("shoot\n");
//    if(root_dir_entry!=NULL)
    free(root_dir_entry);
    return 0;
}
