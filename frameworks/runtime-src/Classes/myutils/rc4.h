#ifndef _RC4_H_  
#  define _RC4_H_  

#  ifdef __cplusplus  
extern "C" {
#  endif /* __cplusplus */  


	/*
	* RC4 context...
	*/

	typedef struct
	{
		unsigned char sbox[256];  /* S boxes for encryption */
		int       i, j;       /* Current indices into S boxes */
	} rc4_context_t;


	/*
	* Prototypes...
	*/

	extern void rc4_init(rc4_context_t *context, const unsigned char *key,
		unsigned keylen);

	extern void rc4_encrypt(rc4_context_t *context, const unsigned char *input,
		unsigned char *output, unsigned len);

#  ifdef __cplusplus  
}
#  endif /* __cplusplus */  

#endif /* !_RC4_H_ */  