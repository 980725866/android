#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/types.h>
#include<stdbool.h>
#include <string.h>

#include <private/android_filesystem_config.h>

#include "binder.h"
#include "test_server.h"

struct binder_state *gb_bs;
uint32_t g_hander;

uint32_t svcmgr_lookup(struct binder_state *bs, uint32_t target, const char *name)
{
    uint32_t handle;
    unsigned iodata[512/4];
    struct binder_io msg, reply;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    bio_put_string16_x(&msg, SVC_MGR_NAME);
    bio_put_string16_x(&msg, name);

    if (binder_call(bs, &msg, &reply, target, SVC_MGR_CHECK_SERVICE))
        return 0;

    handle = bio_get_ref(&reply);

    if (handle)
        binder_acquire(bs, handle);

    binder_done(bs, &msg, &reply);

    return handle;
}

int svcmgr_publish(struct binder_state *bs, uint32_t target, const char *name, void *ptr)
{
    int status;
    unsigned iodata[512/4];
    struct binder_io msg, reply;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    bio_put_string16_x(&msg, SVC_MGR_NAME);
    bio_put_string16_x(&msg, name);
    bio_put_obj(&msg, ptr);

    if (binder_call(bs, &msg, &reply, target, SVC_MGR_ADD_SERVICE))
        return -1;

    status = bio_get_uint32(&reply);

    binder_done(bs, &msg, &reply);

    return status;
}

void sayhello()
{
	int status;
    unsigned iodata[512/4];
    struct binder_io msg, reply;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    
    if (binder_call(gb_bs, &msg, &reply, g_hander, SAYHELLO))
        return -1;
        
    binder_done(gb_bs, &msg, &reply);
}

int sayhello_to(char *name)
{
	int status;
    unsigned iodata[512/4];
    struct binder_io msg, reply;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    bio_put_string16_x(&msg, name);

    if (binder_call(gb_bs, &msg, &reply, g_hander, SAYHELLO_TO))
        return -1;

    status = bio_get_uint32(&reply);

    binder_done(gb_bs, &msg, &reply);
	
	return status;
}

int main(int argc, char **argv)
{
    int ret = 0;
    struct binder_state *bs;
    uint32_t svcmgr = BINDER_SERVICE_MANAGER;
    uint32_t handle;

    bs = binder_open(128*1024);
    if (!bs) {
        fprintf(stderr, "failed to open binder driver\n");
        return -1;
    }
    gb_bs = bs;
    handle = svcmgr_lookup(bs, svcmgr, "hello");
    if (!handle) {
        fprintf(stderr,"cannot get server hello\n");
        return -1;
    }
    g_hander = handle;
    
    if(argc < 2){
    	fprintf(stderr,"please 2 or 3\n");
		return 0;
    }
    else if(argc == 2){
		sayhello();
    }
    else if(argc == 3){
		ret = sayhello_to(argv[2]);
		fprintf(stderr,"ret = %d\n",ret);
    }
    
    fprintf(stderr,"client yes\n");
    binder_release(bs, handle);
    return 0;
}