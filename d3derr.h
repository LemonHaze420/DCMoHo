#ifndef _D3DERR_H_
#define _D3DERR_H_

//
// Traces the appropriate direct draw error.
//

void trace_dd_error(HRESULT dd_err);
void trace_d3d_error(HRESULT dd_err);


#define RET_ERR(call)									\
{														\
	HRESULT rval = (call);								\
	if(rval)											\
	{													\
		char	str[400];								\
		sprintf(str, "d3d error = (%d)\n",rval);		\
		TRACE(str);										\
		trace_d3d_error(rval);							\
		return FALSE;									\
	}													\
}


#endif