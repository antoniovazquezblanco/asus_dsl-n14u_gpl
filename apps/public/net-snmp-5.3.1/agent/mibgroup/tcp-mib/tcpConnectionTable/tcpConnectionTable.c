/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.48 $ of : mfd-top.m2c,v $ 
 *
 * $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/tcp-mib/tcpConnectionTable/tcpConnectionTable.c#1 $
 */
/** \page MFD helper for tcpConnectionTable
 *
 * \section intro Introduction
 * Introductory text.
 *
 */
/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header 
 */
#include "tcpConnectionTable.h"

#include <net-snmp/agent/mib_modules.h>

#include "tcpConnectionTable_interface.h"

oid             tcpConnectionTable_oid[] = { TCPCONNECTIONTABLE_OID };
int             tcpConnectionTable_oid_size =
OID_LENGTH(tcpConnectionTable_oid);

tcpConnectionTable_registration tcpConnectionTable_user_context;

void            initialize_table_tcpConnectionTable(void);
void            shutdown_table_tcpConnectionTable(void);


/**
 * Initializes the tcpConnectionTable module
 */
void
init_tcpConnectionTable(void)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:init_tcpConnectionTable",
                "called\n"));

    /*
     * TODO:300:o: Perform tcpConnectionTable one-time module initialization.
     */

    /*
     * here we initialize all the tables we're planning on supporting
     */
    if (should_init("tcpConnectionTable"))
        initialize_table_tcpConnectionTable();

}                               /* init_tcpConnectionTable */

/**
 * Shut-down the tcpConnectionTable module (agent is exiting)
 */
void
shutdown_tcpConnectionTable(void)
{
    if (should_init("tcpConnectionTable"))
        shutdown_table_tcpConnectionTable();

}

/**
 * Initialize the table tcpConnectionTable 
 *    (Define its contents and how it's structured)
 */
void
initialize_table_tcpConnectionTable(void)
{
    tcpConnectionTable_registration *user_context;
    u_long          flags;

    DEBUGMSGTL(("verbose:tcpConnectionTable:initialize_table_tcpConnectionTable", "called\n"));

    /*
     * TODO:301:o: Perform tcpConnectionTable one-time table initialization.
     */

    /*
     * TODO:302:o: |->Initialize tcpConnectionTable user context
     * if you'd like to pass in a pointer to some data for this
     * table, allocate or set it up here.
     */
    user_context = &tcpConnectionTable_user_context;

    /*
     * No support for any flags yet, but in the future you would
     * set any flags here.
     */
    flags = 0;

    /*
     * call interface initialization code
     */
    _tcpConnectionTable_initialize_interface(user_context, flags);
}                               /* initialize_table_tcpConnectionTable */

/**
 * Shutdown the table tcpConnectionTable 
 */
void
shutdown_table_tcpConnectionTable(void)
{
    /*
     * call interface shutdown code
     */
    _tcpConnectionTable_shutdown_interface
        (&tcpConnectionTable_user_context);
}

/**
 * extra context initialization (eg default values)
 *
 * @param rowreq_ctx    : row request context
 * @param user_init_ctx : void pointer for user (parameter to rowreq_ctx_allocate)
 *
 * @retval MFD_SUCCESS  : no errors
 * @retval MFD_ERROR    : error (context allocate will fail)
 */
int
tcpConnectionTable_rowreq_ctx_init(tcpConnectionTable_rowreq_ctx *
                                   rowreq_ctx, void *user_init_ctx)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_rowreq_ctx_init", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:210:o: |-> Perform extra tcpConnectionTable rowreq initialization. (eg DEFVALS)
     */

    return MFD_SUCCESS;
}                               /* tcpConnectionTable_rowreq_ctx_init */

/**
 * extra context cleanup
 *
 */
void
tcpConnectionTable_rowreq_ctx_cleanup(tcpConnectionTable_rowreq_ctx *
                                      rowreq_ctx)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_rowreq_ctx_cleanup", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:211:o: |-> Perform extra tcpConnectionTable rowreq cleanup.
     */
}                               /* tcpConnectionTable_rowreq_ctx_cleanup */

/**
 * pre-request callback
 *
 * @param user_context
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_ERROR                : other error
 */
int
tcpConnectionTable_pre_request(tcpConnectionTable_registration *
                               user_context)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_pre_request", "called\n"));

    /*
     * TODO:510:o: Perform tcpConnectionTable pre-request actions.
     */

    return MFD_SUCCESS;
}                               /* tcpConnectionTable_pre_request */

/**
 * post-request callback
 *
 * Note:
 *   New rows have been inserted into the container, and
 *   deleted rows have been removed from the container and
 *   released.
 * @param user_context
 * @param rc : MFD_SUCCESS if all requests succeeded
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : other error (ignored)
 */
int
tcpConnectionTable_post_request(tcpConnectionTable_registration *
                                user_context, int rc)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_post_request", "called\n"));

    /*
     * TODO:511:o: Perform tcpConnectionTable post-request actions.
     */

    /*
     * check to set if any rows were changed.
     */
    if (tcpConnectionTable_dirty_get()) {
        /*
         * check if request was successful. If so, this would be
         * a good place to save data to its persistent store.
         */
        if (MFD_SUCCESS == rc) {
            /*
             * save changed rows, if you haven't already
             */
        }

        tcpConnectionTable_dirty_set(0);        /* clear table dirty flag */
    }

    return MFD_SUCCESS;
}                               /* tcpConnectionTable_post_request */


/**********************************************************************
 **********************************************************************
 ***
 *** Table tcpConnectionTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * TCP-MIB::tcpConnectionTable is subid 19 of tcp.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.6.19, length: 8
 */

/*
 * ---------------------------------------------------------------------
 * * TODO:200:r: Implement tcpConnectionTable data context functions.
 */
/*
 * tcpConnectionTable_allocate_data
 *
 * Purpose: create new tcpConnectionTable_data.
 */
tcpConnectionTable_data *
tcpConnectionTable_allocate_data(void)
{
    /*
     * TODO:201:r: |-> allocate memory for the tcpConnectionTable data context.
     */
    tcpConnectionTable_data *rtn =
        SNMP_MALLOC_TYPEDEF(tcpConnectionTable_data);

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_allocate_data", "called\n"));

    if (NULL == rtn) {
        snmp_log(LOG_ERR, "unable to malloc memory for new "
                 "tcpConnectionTable_data.\n");
    }

    return rtn;
}                               /* tcpConnectionTable_allocate_data */

/*
 * tcpConnectionTable_release_data
 *
 * Purpose: release tcpConnectionTable data.
 */
void
tcpConnectionTable_release_data(tcpConnectionTable_data * data)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_release_data", "called\n"));

    /*
     * TODO:202:r: |-> release memory for the tcpConnectionTable data context.
     */
    free(data);
}                               /* tcpConnectionTable_release_data */



/**
 * set mib index(es)
 *
 * @param tbl_idx mib index structure
 * @param tcpConnectionLocalAddressType_val
 * @param tcpConnectionLocalAddress_val_ptr
 * @param tcpConnectionLocalAddress_val_ptr_len
 * @param tcpConnectionLocalPort_val
 * @param tcpConnectionRemAddressType_val
 * @param tcpConnectionRemAddress_val_ptr
 * @param tcpConnectionRemAddress_val_ptr_len
 * @param tcpConnectionRemPort_val
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This convenience function is useful for setting all the MIB index
 *  components with a single function call. It is assume that the C values
 *  have already been mapped from their native/rawformat to the MIB format.
 */
int
tcpConnectionTable_indexes_set_tbl_idx(tcpConnectionTable_mib_index *
                                       tbl_idx,
                                       u_long
                                       tcpConnectionLocalAddressType_val,
                                       char
                                       *tcpConnectionLocalAddress_val_ptr,
                                       size_t
                                       tcpConnectionLocalAddress_val_ptr_len,
                                       u_long tcpConnectionLocalPort_val,
                                       u_long
                                       tcpConnectionRemAddressType_val,
                                       char
                                       *tcpConnectionRemAddress_val_ptr,
                                       size_t
                                       tcpConnectionRemAddress_val_ptr_len,
                                       u_long tcpConnectionRemPort_val)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_indexes_set_tbl_idx", "called\n"));

    /*
     * tcpConnectionLocalAddressType(1)/InetAddressType/ASN_INTEGER/long(u_long)//l/a/w/E/r/d/h 
     */
    /** WARNING: this code might not work for netsnmp_tcpconn_entry */
    /** are cross protocol connections allowed? */
    netsnmp_assert(tbl_idx->tcpConnectionRemAddressType ==
                   tbl_idx->tcpConnectionLocalAddressType);
    if (4 == tcpConnectionLocalAddressType_val)
        tbl_idx->tcpConnectionLocalAddressType = INETADDRESSTYPE_IPV4;
    else if (16 == tcpConnectionLocalAddressType_val)
        tbl_idx->tcpConnectionLocalAddressType = INETADDRESSTYPE_IPV6;
    else
        tbl_idx->tcpConnectionLocalAddressType = INETADDRESSTYPE_UNKNOWN;

    /*
     * tcpConnectionLocalAddress(2)/InetAddress/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h 
     */
    tbl_idx->tcpConnectionLocalAddress_len = sizeof(tbl_idx->tcpConnectionLocalAddress) / sizeof(tbl_idx->tcpConnectionLocalAddress[0]);        /* max length */
    /** WARNING: this code might not work for netsnmp_tcpconn_entry */
    /*
     * make sure there is enough space for tcpConnectionLocalAddress data
     */
    if ((NULL == tbl_idx->tcpConnectionLocalAddress) ||
        (tbl_idx->tcpConnectionLocalAddress_len <
         (tcpConnectionLocalAddress_val_ptr_len))) {
        snmp_log(LOG_ERR, "not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->tcpConnectionLocalAddress_len =
        tcpConnectionLocalAddress_val_ptr_len;
    memcpy(tbl_idx->tcpConnectionLocalAddress,
           tcpConnectionLocalAddress_val_ptr,
           tcpConnectionLocalAddress_val_ptr_len *
           sizeof(tcpConnectionLocalAddress_val_ptr[0]));

    /*
     * tcpConnectionLocalPort(3)/InetPortNumber/ASN_UNSIGNED/u_long(u_long)//l/a/w/e/R/d/H 
     */
    /** WARNING: this code might not work for netsnmp_tcpconn_entry */
    tbl_idx->tcpConnectionLocalPort = tcpConnectionLocalPort_val;

    /*
     * tcpConnectionRemAddressType(4)/InetAddressType/ASN_INTEGER/long(u_long)//l/a/w/E/r/d/h 
     */
    /** WARNING: this code might not work for netsnmp_tcpconn_entry */
    tbl_idx->tcpConnectionRemAddressType =
        tbl_idx->tcpConnectionLocalAddressType;

    /*
     * tcpConnectionRemAddress(5)/InetAddress/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h 
     */
    tbl_idx->tcpConnectionRemAddress_len = sizeof(tbl_idx->tcpConnectionRemAddress) / sizeof(tbl_idx->tcpConnectionRemAddress[0]);      /* max length */
    /** WARNING: this code might not work for netsnmp_tcpconn_entry */
    /*
     * make sure there is enough space for tcpConnectionRemAddress data
     */
    if ((NULL == tbl_idx->tcpConnectionRemAddress) ||
        (tbl_idx->tcpConnectionRemAddress_len <
         (tcpConnectionRemAddress_val_ptr_len))) {
        snmp_log(LOG_ERR, "not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->tcpConnectionRemAddress_len =
        tcpConnectionRemAddress_val_ptr_len;
    memcpy(tbl_idx->tcpConnectionRemAddress,
           tcpConnectionRemAddress_val_ptr,
           tcpConnectionRemAddress_val_ptr_len *
           sizeof(tcpConnectionRemAddress_val_ptr[0]));

    /*
     * tcpConnectionRemPort(6)/InetPortNumber/ASN_UNSIGNED/u_long(u_long)//l/a/w/e/R/d/H 
     */
    /** WARNING: this code might not work for netsnmp_tcpconn_entry */
    tbl_idx->tcpConnectionRemPort = tcpConnectionRemPort_val;


    return MFD_SUCCESS;
}                               /* tcpConnectionTable_indexes_set_tbl_idx */

/**
 * @internal
 * set row context indexes
 *
 * @param reqreq_ctx the row context that needs updated indexes
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This function sets the mib indexs, then updates the oid indexs
 *  from the mib index.
 */
int
tcpConnectionTable_indexes_set(tcpConnectionTable_rowreq_ctx * rowreq_ctx,
                               u_long tcpConnectionLocalAddressType_val,
                               char *tcpConnectionLocalAddress_val_ptr,
                               size_t
                               tcpConnectionLocalAddress_val_ptr_len,
                               u_long tcpConnectionLocalPort_val,
                               u_long tcpConnectionRemAddressType_val,
                               char *tcpConnectionRemAddress_val_ptr,
                               size_t tcpConnectionRemAddress_val_ptr_len,
                               u_long tcpConnectionRemPort_val)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_indexes_set", "called\n"));

    if (MFD_SUCCESS !=
        tcpConnectionTable_indexes_set_tbl_idx(&rowreq_ctx->tbl_idx,
                                               tcpConnectionLocalAddressType_val,
                                               tcpConnectionLocalAddress_val_ptr,
                                               tcpConnectionLocalAddress_val_ptr_len,
                                               tcpConnectionLocalPort_val,
                                               tcpConnectionRemAddressType_val,
                                               tcpConnectionRemAddress_val_ptr,
                                               tcpConnectionRemAddress_val_ptr_len,
                                               tcpConnectionRemPort_val))
        return MFD_ERROR;

    /*
     * convert mib index to oid index
     */
    rowreq_ctx->oid_idx.len = sizeof(rowreq_ctx->oid_tmp) / sizeof(oid);
    if (0 != tcpConnectionTable_index_to_oid(&rowreq_ctx->oid_idx,
                                             &rowreq_ctx->tbl_idx)) {
        return MFD_ERROR;
    }

    return MFD_SUCCESS;
}                               /* tcpConnectionTable_indexes_set */


/*---------------------------------------------------------------------
 * TCP-MIB::tcpConnectionEntry.tcpConnectionState
 * tcpConnectionState is subid 7 of tcpConnectionEntry.
 * Its status is Current, and its access level is ReadWrite.
 * OID: .1.3.6.1.2.1.6.19.1.7
 * Description:
The state of this TCP connection.

            The value listen(2) is included only for parallelism to the
            old tcpConnTable, and should not be used.  A connection in
            LISTEN state should be present in the tcpListenerTable.

            The only value which may be set by a management station is
            deleteTCB(12).  Accordingly, it is appropriate for an agent
            to return a `badValue' response if a management station
            attempts to set this object to any other value.

            If a management station sets this object to the value
            deleteTCB(12), then this has the effect of deleting the TCB
            (as defined in RFC 793) of the corresponding connection on
            the managed node, resulting in immediate termination of the
            connection.

            As an implementation-specific option, a RST segment may be
            sent from the managed node to the other TCP endpoint (note
            however that RST segments are not sent reliably).
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 * Enum range: 9/16. Values:  closed(1), listen(2), synSent(3), synReceived(4), established(5), finWait1(6), finWait2(7), closeWait(8), lastAck(9), closing(10), timeWait(11), deleteTCB(12)
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 */
/**
 * Extract the current value of the tcpConnectionState data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param tcpConnectionState_val_ptr
 *        Pointer to storage for a long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
tcpConnectionState_get(tcpConnectionTable_rowreq_ctx * rowreq_ctx,
                       u_long * tcpConnectionState_val_ptr)
{
   /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != tcpConnectionState_val_ptr);


    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionState_get",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:231:o: |-> Extract the current value of the tcpConnectionState data.
     * copy (* tcpConnectionState_val_ptr ) from rowreq_ctx->data
     */
    (*tcpConnectionState_val_ptr) = rowreq_ctx->data->tcpConnState;

    return MFD_SUCCESS;
}                               /* tcpConnectionState_get */

/*---------------------------------------------------------------------
 * TCP-MIB::tcpConnectionEntry.tcpConnectionProcess
 * tcpConnectionProcess is subid 8 of tcpConnectionEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.2.1.6.19.1.8
 * Description:
The system's process ID for the process associated with 
            this connection, or zero if there is no such process.  This
            value is expected to be the same as HOST-RESOURCES-MIB::
            hrSWRunIndex or SYSAPPL-MIB::sysApplElmtRunIndex for some
            row in the appropriate tables.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is UNSIGNED32 (based on perltype UNSIGNED32)
 * The net-snmp type is ASN_UNSIGNED. The C type decl is u_long (u_long)
 */
/**
 * Extract the current value of the tcpConnectionProcess data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param tcpConnectionProcess_val_ptr
 *        Pointer to storage for a u_long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
tcpConnectionProcess_get(tcpConnectionTable_rowreq_ctx * rowreq_ctx,
                         u_long * tcpConnectionProcess_val_ptr)
{
   /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != tcpConnectionProcess_val_ptr);


    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionProcess_get",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:231:o: |-> Extract the current value of the tcpConnectionProcess data.
     * copy (* tcpConnectionProcess_val_ptr ) from rowreq_ctx->data
     */
    (*tcpConnectionProcess_val_ptr) = rowreq_ctx->data->pid;

    return MFD_SUCCESS;
}                               /* tcpConnectionProcess_get */



/** @} */
/**********************************************************************
 **********************************************************************
 ***
 *** Table tcpConnectionTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * TCP-MIB::tcpConnectionTable is subid 19 of tcp.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.6.19, length: 8
 */
    /*
     * NOTE: if you update this chart, please update the versions in
     *       local/mib2c-conf.d/parent-set.m2i
     *       agent/mibgroup/helpers/baby_steps.c
     * while you're at it.
     */
    /*
     ***********************************************************************
     * Baby Steps Flow Chart (2004.06.05)                                  *
     *                                                                     *
     * +--------------+    +================+    U = unconditional path    *
     * |optional state|    ||required state||    S = path for success      *
     * +--------------+    +================+    E = path for error        *
     ***********************************************************************
     *
     *                        +--------------+
     *                        |     pre      |
     *                        |   request    |
     *                        +--------------+
     *                               | U
     *                        +==============+
     *       +----------------||  object    ||
     *       |              E ||  lookup    ||
     *       |                +==============+
     *       |                       | S
     *       |                +==============+
     *       |              E ||   check    ||
     *       |<---------------||   values   ||
     *       |                +==============+
     *       |                       | S
     *       |                +==============+
     *       |       +<-------||   undo     ||
     *       |       |      E ||   setup    ||
     *       |       |        +==============+
     *       |       |               | S
     *       |       |        +==============+
     *       |       |        ||    set     ||-------------------------->+
     *       |       |        ||   value    || E                         |
     *       |       |        +==============+                           |
     *       |       |               | S                                 |
     *       |       |        +--------------+                           |
     *       |       |        |    check     |-------------------------->|
     *       |       |        |  consistency | E                         |
     *       |       |        +--------------+                           |
     *       |       |               | S                                 |
     *       |       |        +==============+         +==============+  |
     *       |       |        ||   commit   ||-------->||     undo   ||  |
     *       |       |        ||            || E       ||    commit  ||  |
     *       |       |        +==============+         +==============+  |
     *       |       |               | S                     U |<--------+
     *       |       |        +--------------+         +==============+
     *       |       |        | irreversible |         ||    undo    ||
     *       |       |        |    commit    |         ||     set    ||
     *       |       |        +--------------+         +==============+
     *       |       |               | U                     U |
     *       |       +-------------->|<------------------------+
     *       |                +==============+
     *       |                ||   undo     ||
     *       |                ||  cleanup   ||
     *       |                +==============+
     *       +---------------------->| U
     *                        +--------------+
     *                        |    post      |
     *                        |   request    |
     *                        +--------------+
     *
     */

/**
 * Setup up context with information needed to undo a set request.
 *
 * This function will be called before the individual node undo setup
 * functions are called. If you need to do any undo setup that is not
 * related to a specific column, you can do it here.
 *
 * Note that the undo context has been allocated with
 * tcpConnectionTable_allocate_data(), but may need extra
 * initialization similar to what you may have done in
 * tcpConnectionTable_rowreq_ctx_init().
 * Note that an individual node's undo_setup function will only be called
 * if that node is being set to a new value.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in the node's undo_setup
 * function, so it won't be done unless it is necessary.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (tcpConnectionTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 */
int
tcpConnectionTable_undo_setup(tcpConnectionTable_rowreq_ctx * rowreq_ctx)
{
    int             rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_undo_setup",
                "called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:451:M: |-> Setup tcpConnectionTable undo.
     * set up tcpConnectionTable undo information, in preparation for a set.
     * Undo storage is in (* tcpConnectionProcess_val_ptr )*
     */

    return rc;
}                               /* tcpConnectionTable_undo_setup */

/**
 * Undo a set request.
 *
 * This function will be called before the individual node undo
 * functions are called. If you need to do any undo that is not
 * related to a specific column, you can do it here.
 *
 * Note that an individual node's undo function will only be called
 * if that node is being set to a new value.
 *
 * If there is anything  specific to a particular column (e.g. releasing
 * memory for a string), you should do that setup in the node's undo
 * function, so it won't be done unless it is necessary.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (tcpConnectionTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 */
int
tcpConnectionTable_undo(tcpConnectionTable_rowreq_ctx * rowreq_ctx)
{
    int             rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_undo",
                "called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:451:M: |-> tcpConnectionTable undo.
     * tcpConnectionTable undo information, in response to a failed set.
     * Undo storage is in (* tcpConnectionProcess_val_ptr )*
     */

    return rc;
}                               /* tcpConnectionTable_undo_setup */

/**
 * Cleanup up context undo information.
 *
 * This function will be called after set/commit processing. If you
 * allocated any resources in undo_setup, this is the place to release
 * those resources.
 *
 * This function is called regardless of the success or failure of the set
 * request. If you need to perform different steps for cleanup depending
 * on success or failure, you can add a flag to the rowreq_ctx.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (tcpConnectionTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
tcpConnectionTable_undo_cleanup(tcpConnectionTable_rowreq_ctx * rowreq_ctx)
{
    int             rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_undo_cleanup", "called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:452:M: |-> Cleanup tcpConnectionTable undo.
     * Undo storage is in (* tcpConnectionProcess_val_ptr )*
     */

    return rc;
}                               /* tcpConnectionTable_undo_cleanup */

/**
 * commit new values.
 *
 * At this point, you should have done everything you can to ensure that
 * this commit will not fail.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * tcpConnectionTable.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
tcpConnectionTable_commit(tcpConnectionTable_rowreq_ctx * rowreq_ctx)
{
    int             rc = MFD_SUCCESS;
    int             save_flags;

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_commit",
                "called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * save flags, then clear until we actually do something
     */
    save_flags = rowreq_ctx->column_set_flags;
    rowreq_ctx->column_set_flags = 0;

    /*
     * commit tcpConnectionTable data
     * 1) check the column's flag in save_flags to see if it was set.
     * 2) clear the flag when you handle that column
     * 3) set the column's flag in column_set_flags if it needs undo
     *    processing in case of a failure.
     */
    if (save_flags & COLUMN_TCPCONNECTIONSTATE_FLAG) {
        save_flags &= ~COLUMN_TCPCONNECTIONSTATE_FLAG;  /* clear tcpConnectionState */
        /*
         * TODO:482:o: |-> commit column tcpConnectionState.
         */
        rc = -1;
        if (-1 == rc) {
            snmp_log(LOG_ERR,
                     "tcpConnectionTable column tcpConnectionState commit failed\n");
        } else {
            /*
             * set flag, in case we need to undo tcpConnectionState
             */
            rowreq_ctx->column_set_flags |= COLUMN_TCPCONNECTIONSTATE_FLAG;
        }
    }

    /*
     * if we successfully commited this row, set the dirty flag.
     */
    if (MFD_SUCCESS == rc) {
        rowreq_ctx->rowreq_flags |= MFD_ROW_DIRTY;
    }

    if (save_flags) {
        snmp_log(LOG_ERR, "unhandled columns (0x%x) in commit\n",
                 save_flags);
        return MFD_ERROR;
    }

    return rc;
}                               /* tcpConnectionTable_commit */

/**
 * undo commit new values.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * tcpConnectionTable.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
tcpConnectionTable_undo_commit(tcpConnectionTable_rowreq_ctx * rowreq_ctx)
{
    int             rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionTable_undo_commit", "called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:485:M: |-> Undo tcpConnectionTable commit.
     * check the column's flag in rowreq_ctx->column_set_flags to see
     * if it was set during commit, then undo it.
     *
     * eg: if (rowreq_ctx->column_set_flags & COLUMN__FLAG) {}
     */


    /*
     * if we successfully un-commited this row, clear the dirty flag.
     */
    if (MFD_SUCCESS == rc) {
        rowreq_ctx->rowreq_flags &= ~MFD_ROW_DIRTY;
    }

    return rc;
}                               /* tcpConnectionTable_undo_commit */

/*
 * TODO:440:M: Implement tcpConnectionTable node value checks.
 * TODO:450:M: Implement tcpConnectionTable undo functions.
 * TODO:460:M: Implement tcpConnectionTable set functions.
 * TODO:480:M: Implement tcpConnectionTable commit functions.
 */
/*---------------------------------------------------------------------
 * TCP-MIB::tcpConnectionEntry.tcpConnectionState
 * tcpConnectionState is subid 7 of tcpConnectionEntry.
 * Its status is Current, and its access level is ReadWrite.
 * OID: .1.3.6.1.2.1.6.19.1.7
 * Description:
The state of this TCP connection.

            The value listen(2) is included only for parallelism to the
            old tcpConnTable, and should not be used.  A connection in
            LISTEN state should be present in the tcpListenerTable.

            The only value which may be set by a management station is
            deleteTCB(12).  Accordingly, it is appropriate for an agent
            to return a `badValue' response if a management station
            attempts to set this object to any other value.

            If a management station sets this object to the value
            deleteTCB(12), then this has the effect of deleting the TCB
            (as defined in RFC 793) of the corresponding connection on
            the managed node, resulting in immediate termination of the
            connection.

            As an implementation-specific option, a RST segment may be
            sent from the managed node to the other TCP endpoint (note
            however that RST segments are not sent reliably).
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 * Enum range: 9/16. Values:  closed(1), listen(2), synSent(3), synReceived(4), established(5), finWait1(6), finWait2(7), closeWait(8), lastAck(9), closing(10), timeWait(11), deleteTCB(12)
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 */
/**
 * Check that the proposed new value is potentially valid.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param tcpConnectionState_val
 *        A long containing the new value.
 *
 * @retval MFD_SUCCESS        : incoming value is legal
 * @retval MFD_NOT_VALID_NOW  : incoming value is not valid now
 * @retval MFD_NOT_VALID_EVER : incoming value is never valid
 *
 * This is the place to check for requirements that are not
 * expressed in the mib syntax (for example, a requirement that
 * is detailed in the description for an object).
 *
 * You should check that the requested change between the undo value and the
 * new value is legal (ie, the transistion from one value to another
 * is legal).
 *      
 *@note
 * This check is only to determine if the new value
 * is \b potentially valid. This is the first check of many, and
 * is one of the simplest ones.
 * 
 *@note
 * this is not the place to do any checks for values
 * which depend on some other value in the mib. Those
 * types of checks should be done in the
 * tcpConnectionTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *    The value is one of  closed(1), listen(2), synSent(3), synReceived(4), established(5), finWait1(6), finWait2(7), closeWait(8), lastAck(9), closing(10), timeWait(11), deleteTCB(12)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
tcpConnectionState_check_value(tcpConnectionTable_rowreq_ctx * rowreq_ctx,
                               u_long tcpConnectionState_val)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionState_check_value", "called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid tcpConnectionState value.
     */

    return MFD_SUCCESS;         /* tcpConnectionState value not illegal */
}                               /* tcpConnectionState_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (tcpConnectionTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * tcpConnectionTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
tcpConnectionState_undo_setup(tcpConnectionTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionState_undo_setup",
                "called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup tcpConnectionState undo.
     */
    /*
     * copy tcpConnectionState data
     * set rowreq_ctx->undo->tcpConnectionState from rowreq_ctx->data->tcpConnectionState
     */


    return MFD_SUCCESS;
}                               /* tcpConnectionState_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param tcpConnectionState_val
 *        A long containing the new value.
 */
int
tcpConnectionState_set(tcpConnectionTable_rowreq_ctx * rowreq_ctx,
                       u_long tcpConnectionState_val)
{

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionState_set",
                "called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:461:M: |-> Set tcpConnectionState value.
     * set tcpConnectionState value in rowreq_ctx->data
     */

    return MFD_SUCCESS;
}                               /* tcpConnectionState_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
tcpConnectionState_undo(tcpConnectionTable_rowreq_ctx * rowreq_ctx)
{

    DEBUGMSGTL(("verbose:tcpConnectionTable:tcpConnectionState_undo",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up tcpConnectionState undo.
     */
    /*
     * copy tcpConnectionState data
     * set rowreq_ctx->data->tcpConnectionState from rowreq_ctx->undo->tcpConnectionState
     */


    return MFD_SUCCESS;
}                               /* tcpConnectionState_undo */

/**
 * check dependencies
 *
 * This is useful for for tables which have dependencies between columns
 * (or rows, or tables). For example, two columns allocating a percentage
 * of something add up 100%.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * tcpConnectionTable.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @retval MFD_SUCCESS all the changes to the row are legal
 * @retval MFD_ERROR   one or more changes are not legal
 *
 * (see README-table-tcpConnectionTable if you don't have dependencies)
 */
int
tcpConnectionTable_check_dependencies(tcpConnectionTable_rowreq_ctx *
                                      rowreq_ctx)
{
    int             rc = MFD_SUCCESS;

    DEBUGMSGTL(("internal:tcpConnectionTable:tcpConnectionTable_check_dependencies", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:470:o: Check tcpConnectionTable row dependencies.
     * check that all new value are legal and consistent with each other
     */
    return rc;
}                               /* tcpConnectionTable_check_dependencies */

/** @} */
/** @{ */
