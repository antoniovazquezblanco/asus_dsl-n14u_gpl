/*
 * Unix SMB/CIFS implementation.
 * server auto-generated by pidl. DO NOT MODIFY!
 */

#include "includes.h"
#include "../librpc/gen_ndr/srv_echo.h"

static bool api_echo_AddOne(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_AddOne *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_ADDONE];

	r = talloc(talloc_tos(), struct echo_AddOne);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_AddOne, r);
	}

	ZERO_STRUCT(r->out);
	r->out.out_data = talloc_zero(r, uint32_t);
	if (r->out.out_data == NULL) {
		talloc_free(r);
		return false;
	}

	_echo_AddOne(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_AddOne, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_EchoData(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_EchoData *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_ECHODATA];

	r = talloc(talloc_tos(), struct echo_EchoData);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_EchoData, r);
	}

	ZERO_STRUCT(r->out);
	r->out.out_data = talloc_zero_array(r, uint8_t, r->in.len);
	if (r->out.out_data == NULL) {
		talloc_free(r);
		return false;
	}

	_echo_EchoData(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_EchoData, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_SinkData(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_SinkData *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_SINKDATA];

	r = talloc(talloc_tos(), struct echo_SinkData);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_SinkData, r);
	}

	_echo_SinkData(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_SinkData, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_SourceData(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_SourceData *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_SOURCEDATA];

	r = talloc(talloc_tos(), struct echo_SourceData);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_SourceData, r);
	}

	ZERO_STRUCT(r->out);
	r->out.data = talloc_zero_array(r, uint8_t, r->in.len);
	if (r->out.data == NULL) {
		talloc_free(r);
		return false;
	}

	_echo_SourceData(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_SourceData, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_TestCall(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_TestCall *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_TESTCALL];

	r = talloc(talloc_tos(), struct echo_TestCall);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_TestCall, r);
	}

	ZERO_STRUCT(r->out);
	r->out.s2 = talloc_zero(r, const char *);
	if (r->out.s2 == NULL) {
		talloc_free(r);
		return false;
	}

	_echo_TestCall(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_TestCall, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_TestCall2(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_TestCall2 *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_TESTCALL2];

	r = talloc(talloc_tos(), struct echo_TestCall2);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_TestCall2, r);
	}

	ZERO_STRUCT(r->out);
	r->out.info = talloc_zero(r, union echo_Info);
	if (r->out.info == NULL) {
		talloc_free(r);
		return false;
	}

	r->out.result = _echo_TestCall2(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_TestCall2, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_TestSleep(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_TestSleep *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_TESTSLEEP];

	r = talloc(talloc_tos(), struct echo_TestSleep);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_TestSleep, r);
	}

	r->out.result = _echo_TestSleep(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_TestSleep, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_TestEnum(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_TestEnum *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_TESTENUM];

	r = talloc(talloc_tos(), struct echo_TestEnum);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_TestEnum, r);
	}

	ZERO_STRUCT(r->out);
	r->out.foo1 = r->in.foo1;
	r->out.foo2 = r->in.foo2;
	r->out.foo3 = r->in.foo3;
	_echo_TestEnum(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_TestEnum, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_TestSurrounding(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_TestSurrounding *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_TESTSURROUNDING];

	r = talloc(talloc_tos(), struct echo_TestSurrounding);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_TestSurrounding, r);
	}

	ZERO_STRUCT(r->out);
	r->out.data = r->in.data;
	_echo_TestSurrounding(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_TestSurrounding, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_echo_TestDoublePointer(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct echo_TestDoublePointer *r;

	call = &ndr_table_rpcecho.calls[NDR_ECHO_TESTDOUBLEPOINTER];

	r = talloc(talloc_tos(), struct echo_TestDoublePointer);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r, NULL);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(echo_TestDoublePointer, r);
	}

	r->out.result = _echo_TestDoublePointer(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(echo_TestDoublePointer, r);
	}

	push = ndr_push_init_ctx(r, NULL);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}


/* Tables */
static struct api_struct api_rpcecho_cmds[] = 
{
	{"ECHO_ADDONE", NDR_ECHO_ADDONE, api_echo_AddOne},
	{"ECHO_ECHODATA", NDR_ECHO_ECHODATA, api_echo_EchoData},
	{"ECHO_SINKDATA", NDR_ECHO_SINKDATA, api_echo_SinkData},
	{"ECHO_SOURCEDATA", NDR_ECHO_SOURCEDATA, api_echo_SourceData},
	{"ECHO_TESTCALL", NDR_ECHO_TESTCALL, api_echo_TestCall},
	{"ECHO_TESTCALL2", NDR_ECHO_TESTCALL2, api_echo_TestCall2},
	{"ECHO_TESTSLEEP", NDR_ECHO_TESTSLEEP, api_echo_TestSleep},
	{"ECHO_TESTENUM", NDR_ECHO_TESTENUM, api_echo_TestEnum},
	{"ECHO_TESTSURROUNDING", NDR_ECHO_TESTSURROUNDING, api_echo_TestSurrounding},
	{"ECHO_TESTDOUBLEPOINTER", NDR_ECHO_TESTDOUBLEPOINTER, api_echo_TestDoublePointer},
};

void rpcecho_get_pipe_fns(struct api_struct **fns, int *n_fns)
{
	*fns = api_rpcecho_cmds;
	*n_fns = sizeof(api_rpcecho_cmds) / sizeof(struct api_struct);
}

NTSTATUS rpc_rpcecho_dispatch(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx, const struct ndr_interface_table *table, uint32_t opnum, void *_r)
{
	if (cli->pipes_struct == NULL) {
		return NT_STATUS_INVALID_PARAMETER;
	}

	switch (opnum)
	{
		case NDR_ECHO_ADDONE: {
			struct echo_AddOne *r = (struct echo_AddOne *)_r;
			ZERO_STRUCT(r->out);
			r->out.out_data = talloc_zero(mem_ctx, uint32_t);
			if (r->out.out_data == NULL) {
			return NT_STATUS_NO_MEMORY;
			}

			_echo_AddOne(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_ECHODATA: {
			struct echo_EchoData *r = (struct echo_EchoData *)_r;
			ZERO_STRUCT(r->out);
			r->out.out_data = talloc_zero_array(mem_ctx, uint8_t, r->in.len);
			if (r->out.out_data == NULL) {
			return NT_STATUS_NO_MEMORY;
			}

			_echo_EchoData(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_SINKDATA: {
			struct echo_SinkData *r = (struct echo_SinkData *)_r;
			_echo_SinkData(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_SOURCEDATA: {
			struct echo_SourceData *r = (struct echo_SourceData *)_r;
			ZERO_STRUCT(r->out);
			r->out.data = talloc_zero_array(mem_ctx, uint8_t, r->in.len);
			if (r->out.data == NULL) {
			return NT_STATUS_NO_MEMORY;
			}

			_echo_SourceData(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_TESTCALL: {
			struct echo_TestCall *r = (struct echo_TestCall *)_r;
			ZERO_STRUCT(r->out);
			r->out.s2 = talloc_zero(mem_ctx, const char *);
			if (r->out.s2 == NULL) {
			return NT_STATUS_NO_MEMORY;
			}

			_echo_TestCall(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_TESTCALL2: {
			struct echo_TestCall2 *r = (struct echo_TestCall2 *)_r;
			ZERO_STRUCT(r->out);
			r->out.info = talloc_zero(mem_ctx, union echo_Info);
			if (r->out.info == NULL) {
			return NT_STATUS_NO_MEMORY;
			}

			r->out.result = _echo_TestCall2(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_TESTSLEEP: {
			struct echo_TestSleep *r = (struct echo_TestSleep *)_r;
			r->out.result = _echo_TestSleep(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_TESTENUM: {
			struct echo_TestEnum *r = (struct echo_TestEnum *)_r;
			ZERO_STRUCT(r->out);
			r->out.foo1 = r->in.foo1;
			r->out.foo2 = r->in.foo2;
			r->out.foo3 = r->in.foo3;
			_echo_TestEnum(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_TESTSURROUNDING: {
			struct echo_TestSurrounding *r = (struct echo_TestSurrounding *)_r;
			ZERO_STRUCT(r->out);
			r->out.data = r->in.data;
			_echo_TestSurrounding(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		case NDR_ECHO_TESTDOUBLEPOINTER: {
			struct echo_TestDoublePointer *r = (struct echo_TestDoublePointer *)_r;
			r->out.result = _echo_TestDoublePointer(cli->pipes_struct, r);
			return NT_STATUS_OK;
		}

		default:
			return NT_STATUS_NOT_IMPLEMENTED;
	}
}

NTSTATUS rpc_rpcecho_init(void)
{
	return rpc_srv_register(SMB_RPC_INTERFACE_VERSION, "rpcecho", "rpcecho", &ndr_table_rpcecho, api_rpcecho_cmds, sizeof(api_rpcecho_cmds) / sizeof(struct api_struct));
}
