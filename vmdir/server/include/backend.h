/*
 * Copyright © 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the “License”); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */



/*
 * Module Name: Directory Backend
 *
 * Filename: backend.h
 *
 * Abstract:
 *
 * backend interface api
 *
 */

#ifndef __VIDRBACKEND_H__
#define __VIDRBACKEND_H__

#ifdef __cplusplus
extern "C" {
#endif


#define BE_REAL_EID_SIZE(eId)  4 /* (eId <= 0xff ? 1 : (eId <= 0xffff ? 2 : (eId <= 0xffffff ? 3 : (eId <= 0xffffffff ?
                                    4 : 5)))) */

#define BE_DB_MAX_INDEX_ATTRIBUTE       256
#define BE_MAX_DB_NAME_LEN              100

#define BE_DB_FLAGS_ZERO                0
#define BE_DB_TXN_NULL                  NULL
#define BE_DB_PARENT_TXN_NULL           NULL


#define ENTRY_ID_SEQ_INITIAL_VALUE      100
#define ENTRY_ID_SEQ_MAX_VALUE          LONG_MAX
#define ENTRY_ID_SEQ_CACHE_SIZE         100
#define ENTRY_ID_SEQ_KEY                "entryIdSeq"

#define USN_SEQ_INITIAL_VALUE           100
#define USN_SEQ_MAX_VALUE               LONG_MAX /* on a 64 bit machine, it is 9,223,372,036,854,775,807 =>
                                                 With 100 updates/sec in the system, it will take 2924712086 years
                                                 to reach this limit */
#define USN_SEQ_CACHE_SIZE              100
#define USN_SEQ_KEY                     "usnSeq"

#define BE_INDEX_KEY_TYPE_FWD               '0'
#define BE_INDEX_KEY_TYPE_REV               '1'

#define BE_INDEX_OP_TYPE_CREATE             0x0001
#define BE_INDEX_OP_TYPE_DELETE             0x0002
#define BE_INDEX_OP_TYPE_UPDATE             0x0004
#define BE_INDEX_OP_TYPE_UPDATE_SINGLE      0x0008

// meta data buffer allocation size
#define BE_DB_META_NODE_INIT_SIZE           100
#define BE_DB_META_NODE_INC_SIZE            50

#define BE_CANDIDATES_START_ALLOC_SIZE      10

#define BE_OUTSTANDING_USN_LIST_SIZE        32

typedef enum
{
    VDIR_BACKEND_ENTRY_LOCK_READ = 0,
    VDIR_BACKEND_ENTRY_LOCK_WRITE

} VDIR_BACKEND_ENTRY_LOCKTYPE;

typedef enum
{
    VDIR_BACKEND_TXN_READ = 0,
    VDIR_BACKEND_TXN_WRITE

} VDIR_BACKEND_TXN_MODE;

/*
 * Next backend generated entry id
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_MAX_ENTRY_ID)(
                    PVDIR_BACKEND_CTX   pBECtx,
                    ENTRYID*            pEId
                    );
/*
 * Convenient id to entry lookup
 * return error -
 * ERROR_BACKEND_ENTRY_NOT_FOUND:   entry not found
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_SIMPLE_ID_TO_ENTRY)(
                    ENTRYID         eId,
                    PVDIR_ENTRY     pEntry
                    );

/*
 * Convenient Dn to entry lookup
 * return error -
 * ERROR_BACKEND_ENTRY_NOT_FOUND:   entry not found
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_SIMPLE_DN_TO_ENTRY)(
                    PSTR          pszObjectDn,
                    PVDIR_ENTRY   pEntry
                    );

/*
 * ID to entry lookup
 * return error -
 * ERROR_BACKEND_ENTRY_NOT_FOUND:   entry not found
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_ID_TO_ENTRY)(
                    PVDIR_BACKEND_CTX           pBECtx,
                    PVDIR_SCHEMA_CTX            pSchemaCtx,
                    ENTRYID                     eId,
                    PVDIR_ENTRY                 pEntry,
                    VDIR_BACKEND_ENTRY_LOCKTYPE lockType
                    );
/*
 * DN to entry lookup
 * return error -
 * ERROR_BACKEND_ENTRY_NOT_FOUND:   entry not found
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_DN_TO_ENTRY)(
                    PVDIR_BACKEND_CTX           pBECtx,
                    PVDIR_SCHEMA_CTX            pSchemaCtx,
                    VDIR_BERVALUE*              pDn,
                    PVDIR_ENTRY                 pEntry,
                    VDIR_BACKEND_ENTRY_LOCKTYPE lockType
                    );
/*
 * DN to entry id lookup
 * return error -
 * ERROR_BACKEND_ENTRY_NOT_FOUND:   entry not found
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_DN_TO_ENTRY_ID)(
                    PVDIR_BACKEND_CTX           pBECtx,
                    VDIR_BERVALUE*              pDn,
                    ENTRYID*                    pEId
                    );
/*
 * Add entry
 * return error -
 * ERROR_BACKEND_CONSTRAINT:        attribute value constraint violation
 * ERROR_BACKEND_PARENT_NOTFOUND:   parent not exists
 * ERROR_BACKEND_ENTRY_EXISTS:      entry already exists
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_ENTRY_ADD)(
                    PVDIR_BACKEND_CTX   pBECtx,
                    PVDIR_ENTRY         pEntry
                    );
/*
 * Check DN reference
 * return error -
 * ERROR_BACKEND_CONSTRAINT:        reference dn not found
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_CHK_DN_REFERENCE)(
                    PVDIR_BACKEND_CTX   pBECtx,
                    PVDIR_ENTRY         pEntry
                    );
/*
 * Delete entry
 * return error -
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_ENTRY_DELETE)(
                    PVDIR_BACKEND_CTX   pBECtx,
                    PVDIR_MODIFICATION  pMods,
                    PVDIR_ENTRY         pEntry
                    );
/*
 * Modify entry
 * return error -
 * ERROR_BACKEND_CONSTRAINT:        attribute value constraint violation
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_ENTRY_MODIFY)(
                    PVDIR_BACKEND_CTX   pBECtx,
                    VDIR_MODIFICATION*  pMods,
                    PVDIR_ENTRY         pEntry
                    );
/*
 * Check if leaf entry
 * return error -
 * ERROR_BACKEND_DEADLOCK:          deadlock
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_CHK_IS_LEAF_ENTRY)(
                    PVDIR_BACKEND_CTX   pBECtx,
                    ENTRYID             eId,
                    PBOOLEAN            pIsLeafEntry
                    );
/*
 * Add indices for a range of entries
 * return error -
 * ERROR_BACKEND_ENTRY_NOTFOUND:    no entry found
 * ERROR_BACKEND_ERROR:             all others
 */
//ERROR_BACKEND_ENTRY_NOTFOUND
typedef DWORD (*PFN_BACKEND_GET_CANDIDATES)(
                    PVDIR_BACKEND_CTX   pBECtx,
                    VDIR_FILTER*        pFilter
                    );
/*
 * Add indices for a range of entries
 * return error -
 * ERROR_BACKEND_MAX_RETRY:         max deadlock retry fail
 * ERROR_DATA_CONSTRAIN_VIOLATION:  data constraint violation
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_ENTRY_ADD_INDICES)(
                    PVDIR_CFG_ATTR_INDEX_DESC*  ppIndexDesc,
                    DWORD   dwNumIndices,
                    DWORD   dwStartEntryId,
                    DWORD   dwBatchSize
                    );
/*
 * Begin a transaction
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_TXN_BEGIN)(
                    PVDIR_BACKEND_CTX        pBECtx,
                    VDIR_BACKEND_TXN_MODE    txnMode
                    );
/*
 * Abort a transaction
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_TXN_ABORT)(
                    PVDIR_BACKEND_CTX   pBECtx
                    );
/*
 * Commit a transaction
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_TXN_COMMIT)(
                    PVDIR_BACKEND_CTX   pBECtx
                    );
/*
 * Initialize backend
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_INIT)(
                    VOID
                    );
/*
 * Open database
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_DB_OPEN)(
                    VOID
                    );
/*
 * Open index database
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_INDEX_OPEN)(
                    VOID
                    );
/*
 * Add new index database
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_INDEX_ADD)(
                    PVDIR_CFG_ATTR_INDEX_DESC   pIndexDesc
                    );
/*
 * Shutdown backend
 * return error -
 * ERROR_BACKEND_ERROR:             all others
 */
typedef DWORD (*PFN_BACKEND_SHUTDOWN)(
                    VOID
                    );

typedef DWORD (*PFN_BACKEND_GET_ATTR_META_DATA)(
                    PVDIR_BACKEND_CTX,
                    VDIR_ATTRIBUTE *,
                    ENTRYID
                    );

typedef DWORD (*PFN_BACKEND_GET_ALL_ATTR_META_DATA)(
                    PVDIR_BACKEND_CTX,
                    ENTRYID,
                    PATTRIBUTE_META_DATA_NODE *,
                    int *
                    );

typedef DWORD (*PFN_BACKEND_GET_NEXT_USN)(
                    PVDIR_BACKEND_CTX,
                    USN *   pUSN);

typedef USN (*PFN_BACKEND_GET_LEAST_OUTSTANDING_USN)(
                    PVDIR_BACKEND_CTX,
                    BOOLEAN);

typedef USN (*PFN_BACKEND_GET_HIGHEST_COMMITTED_USN)(
                    PVDIR_BACKEND_CTX);

typedef USN (*PFN_BACKEND_GET_MAX_ORIGINATING_USN)(
                    PVDIR_BACKEND_CTX);


typedef VOID (*PFN_BACKEND_SET_MAX_ORIGINATING_USN)(
                    PVDIR_BACKEND_CTX, USN);

typedef DWORD (*PFN_BACKEND_STRKEY_GET_VALUES)(
                    PVDIR_BACKEND_CTX,
                    PCSTR,
                    PVMDIR_STRING_LIST*);

typedef DWORD (*PFN_BACKEND_STRKEY_SET_VALUES)(
                    PVDIR_BACKEND_CTX,
                    PCSTR,
                    PVMDIR_STRING_LIST);

typedef DWORD (*PFN_BACKEND_CONFIGURE_FSYNC)(
                    BOOLEAN);

typedef struct _VDIR_BACKEND_USN_LIST*   PVDIR_BACKEND_USN_LIST;

/*******************************************************************************
 * if success, interface function return 0.
 * if fail, interface function return ERROR_BACKEND_XXX. also, VDIR_BACKEND_CTX should
 *      contain backend specific error code and error message.
 ******************************************************************************/
typedef struct _VDIR_BACKEND_INTERFACE
{
    // NOTE: order of fields MUST stay in sync with struct initializer...

    //////////////////////////////////////////////////////////////////////
    // backend life cycle functions
    // 1. start up calling order: beInit->beDBOpen->beIndexOpen
    // 2. live session: could call beIndexAdd to add new indices
    // 3. shutdown calling order: beShutdown
    //////////////////////////////////////////////////////////////////////
    /*
     * initialize backend
     */
    PFN_BACKEND_INIT                pfnBEInit;
    /*
     * open database
     */
    PFN_BACKEND_DB_OPEN             pfnBEDBOpen;
    /*
     * open index database
     */
    PFN_BACKEND_INDEX_OPEN          pfnBEIndexOpen;
    /*
     * add a new index database
     */
    PFN_BACKEND_INDEX_ADD           pfnBEIndexAdd;
    /*
     * shutdown backend
     */
    PFN_BACKEND_SHUTDOWN            pfnBEShutdown;

    //////////////////////////////////////////////////////////////////////
    // transaction related functions
    // NO nested transaction support currently.
    //////////////////////////////////////////////////////////////////////
    /*
     * begin transaction
     */
    PFN_BACKEND_TXN_BEGIN           pfnBETxnBegin;
    /*
     * abort transaction
     */
    PFN_BACKEND_TXN_ABORT           pfnBETxnAbort;
    /*
     * commit transaction
     */
    PFN_BACKEND_TXN_COMMIT          pfnBETxnCommit;

    //////////////////////////////////////////////////////////////////////
    // entry search/read/write functions
    //////////////////////////////////////////////////////////////////////
    /*
     * get entry with ID
     */
    PFN_BACKEND_SIMPLE_ID_TO_ENTRY    pfnBESimpleIdToEntry;

    /*
     * get entry with DN
     */
    PFN_BACKEND_SIMPLE_DN_TO_ENTRY    pfnBESimpleDnToEntry;
    /*
     * get entry with ID
     */
    PFN_BACKEND_ID_TO_ENTRY           pfnBEIdToEntry;
    /*
     * get entry with DN
     */
    PFN_BACKEND_DN_TO_ENTRY           pfnBEDNToEntry;
    /*
     * get ID with DN
     */
    PFN_BACKEND_DN_TO_ENTRY_ID        pfnBEDNToEntryId;
    /*
     * verify DN referenced attribute integrity, i.e. referred DN exists
     */
    PFN_BACKEND_CHK_DN_REFERENCE      pfnBEChkDNReference;
    /*
     * check if entry is a leaf node
     */
    PFN_BACKEND_CHK_IS_LEAF_ENTRY     pfnBEChkIsLeafEntry;
    /*
     * get candidate with filter
     */
    PFN_BACKEND_GET_CANDIDATES        pfnBEGetCandidates;
    /*
     * add entry
     */
    PFN_BACKEND_ENTRY_ADD             pfnBEEntryAdd;
    /*
     * delete entry
     */
    PFN_BACKEND_ENTRY_DELETE          pfnBEEntryDelete;
    /*
     * modify entry
     */
    PFN_BACKEND_ENTRY_MODIFY          pfnBEEntryModify;
    /*
     *  add new indices for a range of entries
     *  called via indexingthr to scan whole db to add new indices
     */
    PFN_BACKEND_ENTRY_ADD_INDICES     pfnBEEntryAddIndices;

    //////////////////////////////////////////////////////////////////////
    // entry id (sequence) function
    //////////////////////////////////////////////////////////////////////
    /*
     * get the maximum entry id in the system
     */
    PFN_BACKEND_MAX_ENTRY_ID         pfnBEMaxEntryId;

    //////////////////////////////////////////////////////////////////////
    // function to read attribute meta data
    //////////////////////////////////////////////////////////////////////
    /*
     * Read attribute meta data for a particular attribute of an entry
     */
    PFN_BACKEND_GET_ATTR_META_DATA  pfnBEGetAttrMetaData;

    //////////////////////////////////////////////////////////////////////
    // function to read attribute meta data for all the attributes of an entry
    //////////////////////////////////////////////////////////////////////
    /*
     * Read attribute meta data for all attributes of an entry
     */
    PFN_BACKEND_GET_ALL_ATTR_META_DATA  pfnBEGetAllAttrsMetaData;

    //////////////////////////////////////////////////////////////////////
    // function to read next USN from a sequence (e.g.) in a backend
    //////////////////////////////////////////////////////////////////////
    /*
     * Get next USN from a backend sequence
     */
    PFN_BACKEND_GET_NEXT_USN        pfnBEGetNextUSN;

    //////////////////////////////////////////////////////////////////////
    // generic read/write functions
    //////////////////////////////////////////////////////////////////////
    /*
     * Use a generic db to serve key/value pair storage
     * This db allows dup key.
     * This db compare key lexically.
     */
    PFN_BACKEND_STRKEY_GET_VALUES           pfnBEStrkeyGetValues;

    /*
     * Use a generic db to serve key/value pair storage
     */
    PFN_BACKEND_STRKEY_SET_VALUES           pfnBEStrKeySetValues;

    //////////////////////////////////////////////////////////////////////
    // configuration functions
    //////////////////////////////////////////////////////////////////////
    /*
     * Turn fsync on/off
     */
    PFN_BACKEND_CONFIGURE_FSYNC             pfnBEConfigureFsync;

    //////////////////////////////////////////////////////////////////////
    // function to get the least outstanding USN in BACKEND_USN_LIST
    // replication can trust and search USN change below this number
    //////////////////////////////////////////////////////////////////////
    /*
     * Get least outstanding USN
     */
    PFN_BACKEND_GET_LEAST_OUTSTANDING_USN   pfnBEGetLeastOutstandingUSN;

    //////////////////////////////////////////////////////////////////////
    // function to get the highest committed USN
    //////////////////////////////////////////////////////////////////////
    /*
     * Get highest committed USN
     */
    PFN_BACKEND_GET_HIGHEST_COMMITTED_USN   pfnBEGetHighestCommittedUSN;

    //////////////////////////////////////////////////////////////////////
    // function to get the maximum originating USN
    //////////////////////////////////////////////////////////////////////
    /*
     * Get Max Originating USN
     */
    PFN_BACKEND_GET_MAX_ORIGINATING_USN   pfnBEGetMaxOriginatingUSN;

    //////////////////////////////////////////////////////////////////////
    // function to set the maximum originating USN
    //////////////////////////////////////////////////////////////////////
    /*
     * Set Max Originating USN
     */
    PFN_BACKEND_SET_MAX_ORIGINATING_USN   pfnBESetMaxOriginatingUSN;

    //////////////////////////////////////////////////////////////////////
    // Structure to hold all outstanding USNs
    //////////////////////////////////////////////////////////////////////
    PVDIR_BACKEND_USN_LIST                  pBEUSNList;

} VDIR_BACKEND_INTERFACE;

typedef struct _VDIR_BACKEND_GLOBALS
{
    // NOTE: order of fields MUST stay in sync with struct initializer...
    PCSTR                           pszBERootDN;
    PVDIR_BACKEND_INTERFACE         pBE;
    USN                             usnFirstNext;

} VDIR_BACKEND_GLOBALS, *PVDIR_BACKEND_GLOBALS;

// backend.c
DWORD
VmDirBackendConfig(
    VOID);

VOID
VmDirBackendGetFirstNextUSN(
    USN *pUSN
    );

PVDIR_BACKEND_INTERFACE
VmDirBackendSelect(
    PCSTR   pszDN);

VOID
VmDirBackendContentFree(
    PVDIR_BACKEND_INTERFACE     pBE
    );

VOID
VmDirBackendCtxFree(
    PVDIR_BACKEND_CTX   pBECtx
    );

VOID
VmDirBackendCtxContentFree(
    PVDIR_BACKEND_CTX   pBECtx
    );

DWORD
VmDirBackendInitUSNList(
    PVDIR_BACKEND_INTERFACE   pBE
    );

VOID
VmDirBackendSetMaxOutstandingUSN(
    PVDIR_BACKEND_CTX   pBECtx,
    USN                 nextAvailableUSN
    );

DWORD
VmDirBackendAddOutstandingUSN(
    PVDIR_BACKEND_CTX      pBECtx
    );

VOID
VmDirBackendRemoveOutstandingUSN(
    PVDIR_BACKEND_CTX      pBECtx
    );

DWORD
VmDirBackendAddOriginatingUSN(
    PVDIR_BACKEND_CTX      pBECtx
    );

VOID
VmDirBackendRemoveOriginatingUSN(
    PVDIR_BACKEND_CTX      pBECtx
    );


// util.c
DWORD
VmDirSimpleNormDNToEntry(
    PCSTR           pszNormDN,
    PVDIR_ENTRY*    ppEntry
    );

DWORD
VmDirSimpleDNToEntry(
    PCSTR           pszDN,
    PVDIR_ENTRY*    ppEntry
    );

DWORD
VmDirIsGroupHasThisMember(
    PVDIR_OPERATION    pOperation, /* Optional */
    PSTR               pszGroupDN,
    PSTR               pszTargetDN,
    PVDIR_ENTRY        pGroupEntry, /* Optional */
    PBOOLEAN           pbIsGroupMember
    );


#ifdef __cplusplus
}
#endif

#endif /* __VIDRBACKEND_H__ */
