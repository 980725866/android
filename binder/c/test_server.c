
/* Copyright 2008 The Android Open Source Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/types.h>
#include<stdbool.h>
#include <string.h>

#include <private/android_filesystem_config.h>

#include "binder.h"
#include "test_server.h"

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
	static int ret = 0;
	fprintf(stderr, "cnt = %d\n",++ret);
}

int sayhello_to(char *name)
{
	static int status = 0;
	fprintf(stderr, "name = %s\n",name);
	status++;
	return status;
}

int hello_handler(struct binder_state *bs,
                   struct binder_transaction_data *txn,
                   struct binder_io *msg,
                   struct binder_io *reply)
{
    struct svcinfo *si;
    uint16_t *s;
    size_t len;
    uint32_t handle;
    uint32_t strict_policy;
    int i;
    char name[512];
	strict_policy = bio_get_uint32(msg);
    
    switch(txn->code) {
    case SAYHELLO:
        sayhello();
        return 0;

    case SAYHELLO_TO:
    	s = bio_get_string16(msg, &len);
		if (s == NULL) {
			return -1;
		}
		for (i = 0; i < len; i++)
			name[i] = s[i];
		name[i] = '\0';
        i = sayhello_to(name);
        bio_put_uint32(reply, i);
        break ;
    default:
        fprintf(stderr,"unknown code %d\n", txn->code);
        return -1;
    }
    return 0;
}


int main(int argc, char **argv)
{
    int ret;
    struct binder_state *bs;
    uint32_t svcmgr = BINDER_SERVICE_MANAGER;
    uint32_t handle;

    bs = binder_open(128*1024);
    if (!bs) {
        fprintf(stderr, "failed to open binder driver\n");
        return -1;
    }
    ret = svcmgr_publish(bs, svcmgr, "hello",hello_handler);
    if (ret) {
        fprintf(stderr,"cannot add server hello\n");
        return -1;
    }
    fprintf(stderr,"server yes\n");
    
    binder_loop(bs, hello_handler); 
    
    return 0;
}

