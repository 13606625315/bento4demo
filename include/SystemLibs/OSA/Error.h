#ifndef __IMOUOS_OSA_ERROR_H__
#define __IMOUOS_OSA_ERROR_H__

typedef enum
{
    E_IMOUOS_SUCCESS                    = 0, /* no error, success */    
    E_IMOUOS_ERR_FAILED                 = -1   ,/* unexpected error */
    E_IMOUOS_ERR_ILLEGAL_PARAM          = -2, /* at lease one parameter is illagal
                                         ** eg, an illegal enumeration value             */
    E_IMOUOS_ERR_EXIST                  = -3, /* resource exists                              */
    E_IMOUOS_ERR_UNEXIST                = -4,/* resource unexists                            */
    E_IMOUOS_ERR_NULL_PTR               = -5, /* using a NULL point                           */
    E_IMOUOS_ERR_NOT_CONFIG             = -6, /* try to enable or initialize system, device
                                         ** or channel, before configing attribute       */
    E_IMOUOS_ERR_NOT_SUPPORT            = -7, /* operation or type is not supported by NOW    */
    E_IMOUOS_ERR_NOT_PERM               = -8, /* operation is not permitted
                                         ** eg, try to change static attribute           */
    E_IMOUOS_ERR_NOMEM                  = -9,/* failure caused by malloc memory              */
    E_IMOUOS_ERR_SYS_NOTREADY           = -10,/* System is not ready,maybe not initialed or
                                         ** loaded. Returning the error code when opening
                                         ** a device file failed.                        */
    E_IMOUOS_ERR_BUSY                   = -11,/* resource is busy,
                                         ** eg. destroy a venc chn without unregister it */
    E_IMOUOS_ERR_NOT_INIT               = -12,/* module not init before use it*/
    E_IMOUOS_ERR_INITED                 = -13,/* module already init*/
    E_IMOUOS_ERR_SYS_TIMEOUT            = -14,/* sys timeout*/
    E_IMOUOS_ERR_BUF_OVERFLOW           = -15, /* no buffer for new data*/ 
    E_IMOUOS_ERR_DEAD_OBJECT			= -16, /* dead binder object*/
    E_IMOUOS_ERR_TIMED_OUT				= -17,	/* timeout event*/
    E_IMOUOS_ERR_FAILED_TRANSACTION		= -18,	/* bind transact failed event*/
    E_IMOUOS_ERR_INVALID_OPERATION		= -19,	/* bind invalid operation*/
    E_IMOUOS_ERR_UNKNOWN				= -20,	/* binder unknow err*/
    E_IMOUOS_ERR_NO_ENTRY               = -21,  /* No such file or director*/
    E_IMOUOS_ERR_SRCH                   = -22,  /* No such process*/
    E_IMOUOS_ERR_IO                     = -23,  /* io error*/
    E_IMOUOS_ERR_NXIO                   = -24,  /* No such device or address*/
    E_IMOUOS_ERR_AGAIN                  = -25,  /* try again*/
    E_IMOUOS_ERR_NOSPC                  = -26,  /* no space*/
    E_IMOUOS_ERR_NOTDIR                 = -27,  /* not dir*/
    E_IMOUOS_ERR_PROTO                  = -28,  /* proto error*/

    E_IMOUOS_ERR_MAX                    = -127,/* maxium code, private error code of all modules
                                          ** must be greater than it                      */
}IMOUOS_ErrCode_e;

#endif
