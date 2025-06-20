
bi_mul.c.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000120 <karatsuba_multiply>:
# static bigint *karatsuba_multiply(
# 	const bigint *const restrict n1, const bigint *const restrict n2
# )
# {
 120:	55                   	push   rbp
 121:	41 57                	push   r15
 123:	41 56                	push   r14
 125:	41 55                	push   r13
 127:	41 54                	push   r12
 129:	53                   	push   rbx
 12a:	48 83 ec 68          	sub    rsp,0x68
# 	if ((!n1 || !n2) || (n1->len < 1 || n2->len < 1) || (!n1->num || !n2->num))
 12e:	48 85 ff             	test   rdi,rdi
 131:	0f 94 c0             	sete   al
 134:	48 85 f6             	test   rsi,rsi
 137:	0f 94 c1             	sete   cl
 13a:	08 c1                	or     cl,al
 13c:	75 70                	jne    1ae <karatsuba_multiply+0x8e>
 13e:	48 8b 17             	mov    rdx,QWORD PTR [rdi]
 141:	48 d1 fa             	sar    rdx,1
 144:	48 85 d2             	test   rdx,rdx
 147:	7e 65                	jle    1ae <karatsuba_multiply+0x8e>
 149:	48 8b 06             	mov    rax,QWORD PTR [rsi]
 14c:	48 d1 f8             	sar    rax,1
 14f:	48 85 c0             	test   rax,rax
 152:	7e 5a                	jle    1ae <karatsuba_multiply+0x8e>
 154:	4c 8b 47 08          	mov    r8,QWORD PTR [rdi+0x8]
 158:	4d 85 c0             	test   r8,r8
 15b:	74 51                	je     1ae <karatsuba_multiply+0x8e>
 15d:	48 8b 4e 08          	mov    rcx,QWORD PTR [rsi+0x8]
 161:	48 85 c9             	test   rcx,rcx
 164:	74 48                	je     1ae <karatsuba_multiply+0x8e>
# 		return (NULL);
#
# 	if (n1->len < KARATSUBA_GAIN_CUTOFF || n2->len < KARATSUBA_GAIN_CUTOFF)
 166:	48 83 fa 15          	cmp    rdx,0x15
 16a:	41 0f 93 c1          	setae  r9b
 16e:	48 83 f8 15          	cmp    rax,0x15
 172:	41 0f 93 c2          	setae  r10b
 176:	45 84 d1             	test   r9b,r10b
 179:	75 44                	jne    1bf <karatsuba_multiply+0x9f>
 17b:	48 89 f3             	mov    rbx,rsi
 17e:	48 89 7c 24 08       	mov    QWORD PTR [rsp+0x8],rdi
# 	if (bi_iszero(n1) || bi_iszero(n2))
 183:	e8 00 00 00 00       	call   188 <karatsuba_multiply+0x68>
			184: R_X86_64_PLT32	bi_iszero-0x4
 188:	84 c0                	test   al,al
 18a:	75 10                	jne    19c <karatsuba_multiply+0x7c>
 18c:	48 89 df             	mov    rdi,rbx
 18f:	e8 00 00 00 00       	call   194 <karatsuba_multiply+0x74>
			190: R_X86_64_PLT32	bi_iszero-0x4
 194:	84 c0                	test   al,al
 196:	0f 84 13 02 00 00    	je     3af <karatsuba_multiply+0x28f>
# 		return (_bi_alloc(1));
 19c:	bf 01 00 00 00       	mov    edi,0x1
 1a1:	e8 00 00 00 00       	call   1a6 <karatsuba_multiply+0x86>
			1a2: R_X86_64_PLT32	_bi_alloc-0x4
 1a6:	49 89 c6             	mov    r14,rax
 1a9:	e9 87 03 00 00       	jmp    535 <karatsuba_multiply+0x415>
 1ae:	31 c0                	xor    eax,eax
# 	_bi_free(xhi_plus_xlo);
# 	_bi_free(yhi_plus_ylo);
# 	_bi_free(z0);
# 	_bi_free(z3);
# 	return (result);
# }
 1b0:	48 83 c4 68          	add    rsp,0x68
 1b4:	5b                   	pop    rbx
 1b5:	41 5c                	pop    r12
 1b7:	41 5d                	pop    r13
 1b9:	41 5e                	pop    r14
 1bb:	41 5f                	pop    r15
 1bd:	5d                   	pop    rbp
 1be:	c3                   	ret
# 	const len_ty i = (n1->len > n2->len ? n1->len / 2 : n2->len / 2);
 1bf:	48 39 c2             	cmp    rdx,rax
 1c2:	49 89 c1             	mov    r9,rax
 1c5:	4c 0f 47 ca          	cmova  r9,rdx
 1c9:	4c 89 cd             	mov    rbp,r9
 1cc:	48 d1 ed             	shr    rbp,1
# 	if (i >= n->len)
 1cf:	48 89 d6             	mov    rsi,rdx
 1d2:	48 29 ee             	sub    rsi,rbp
 1d5:	76 13                	jbe    1ea <karatsuba_multiply+0xca>
# 		split.hi.len = n->len - i;
 1d7:	48 01 f6             	add    rsi,rsi
 1da:	48 89 74 24 10       	mov    QWORD PTR [rsp+0x10],rsi
# 		split.hi.num = &(n->num[i]);
 1df:	49 8d 34 a8          	lea    rsi,[r8+rbp*4]
 1e3:	48 89 74 24 18       	mov    QWORD PTR [rsp+0x18],rsi
 1e8:	eb 0c                	jmp    1f6 <karatsuba_multiply+0xd6>
# 		split.hi = zero;
 1ea:	0f 10 05 00 00 00 00 	movups xmm0,XMMWORD PTR [rip+0x0]        # 1f1 <karatsuba_multiply+0xd1>
			1ed: R_X86_64_PC32	.data.rel.ro-0x4
 1f1:	0f 29 44 24 10       	movaps XMMWORD PTR [rsp+0x10],xmm0
# 		split.lo.len = i < n->len ? i : n->len;
 1f6:	48 39 d5             	cmp    rbp,rdx
 1f9:	48 0f 42 d5          	cmovb  rdx,rbp
 1fd:	48 8d 7c 24 20       	lea    rdi,[rsp+0x20]
 202:	48 01 d2             	add    rdx,rdx
 205:	48 89 54 24 20       	mov    QWORD PTR [rsp+0x20],rdx
# 		split.lo.num = n->num;
 20a:	4c 89 44 24 28       	mov    QWORD PTR [rsp+0x28],r8
# 	if (i >= n->len)
 20f:	48 89 c2             	mov    rdx,rax
 212:	48 29 ea             	sub    rdx,rbp
 215:	4c 89 4c 24 38       	mov    QWORD PTR [rsp+0x38],r9
 21a:	76 13                	jbe    22f <karatsuba_multiply+0x10f>
# 		split.hi.len = n->len - i;
 21c:	48 01 d2             	add    rdx,rdx
 21f:	48 89 54 24 40       	mov    QWORD PTR [rsp+0x40],rdx
# 		split.hi.num = &(n->num[i]);
 224:	48 8d 14 a9          	lea    rdx,[rcx+rbp*4]
 228:	48 89 54 24 48       	mov    QWORD PTR [rsp+0x48],rdx
 22d:	eb 0c                	jmp    23b <karatsuba_multiply+0x11b>
# 		split.hi = zero;
 22f:	0f 10 05 00 00 00 00 	movups xmm0,XMMWORD PTR [rip+0x0]        # 236 <karatsuba_multiply+0x116>
			232: R_X86_64_PC32	.data.rel.ro-0x4
 236:	0f 29 44 24 40       	movaps XMMWORD PTR [rsp+0x40],xmm0
# 		split.lo.len = i < n->len ? i : n->len;
 23b:	48 39 c5             	cmp    rbp,rax
 23e:	48 0f 42 c5          	cmovb  rax,rbp
 242:	4c 8d 6c 24 50       	lea    r13,[rsp+0x50]
 247:	48 01 c0             	add    rax,rax
 24a:	48 89 44 24 50       	mov    QWORD PTR [rsp+0x50],rax
# 		split.lo.num = n->num;
 24f:	48 89 4c 24 58       	mov    QWORD PTR [rsp+0x58],rcx
# 	bigint *const restrict z0 = karatsuba_multiply(&x.lo, &y.lo);
 254:	4c 89 ee             	mov    rsi,r13
 257:	e8 c4 fe ff ff       	call   120 <karatsuba_multiply>
 25c:	48 89 c3             	mov    rbx,rax
 25f:	4c 8d 74 24 10       	lea    r14,[rsp+0x10]
 264:	4c 8d 64 24 40       	lea    r12,[rsp+0x40]
# 	bigint *const restrict z2 = karatsuba_multiply(&x.hi, &y.hi);
 269:	4c 89 f7             	mov    rdi,r14
 26c:	4c 89 e6             	mov    rsi,r12
 26f:	e8 ac fe ff ff       	call   120 <karatsuba_multiply>
 274:	49 89 c7             	mov    r15,rax
# 	bigint *const restrict xhi_plus_xlo = bi_add(&x.hi, &x.lo);
 277:	4c 89 f7             	mov    rdi,r14
 27a:	48 8d 74 24 20       	lea    rsi,[rsp+0x20]
 27f:	e8 00 00 00 00       	call   284 <karatsuba_multiply+0x164>
			280: R_X86_64_PLT32	bi_add-0x4
 284:	49 89 c6             	mov    r14,rax
# 	bigint *const restrict yhi_plus_ylo = bi_add(&y.hi, &y.lo);
 287:	4c 89 e7             	mov    rdi,r12
 28a:	4c 89 ee             	mov    rsi,r13
 28d:	e8 00 00 00 00       	call   292 <karatsuba_multiply+0x172>
			28e: R_X86_64_PLT32	bi_add-0x4
 292:	49 89 c4             	mov    r12,rax
 295:	4c 89 74 24 08       	mov    QWORD PTR [rsp+0x8],r14
# 	bigint *restrict z3 = karatsuba_multiply(xhi_plus_xlo, yhi_plus_ylo);
 29a:	4c 89 f7             	mov    rdi,r14
 29d:	4d 89 fe             	mov    r14,r15
 2a0:	49 89 df             	mov    r15,rbx
 2a3:	48 89 c6             	mov    rsi,rax
 2a6:	e8 75 fe ff ff       	call   120 <karatsuba_multiply>
 2ab:	49 89 c5             	mov    r13,rax
# 	if (!z0 || !z2 || !z3)
 2ae:	48 85 db             	test   rbx,rbx
 2b1:	0f 95 c0             	setne  al
 2b4:	4d 85 f6             	test   r14,r14
 2b7:	0f 95 c1             	setne  cl
 2ba:	20 c1                	and    cl,al
 2bc:	4d 85 ed             	test   r13,r13
 2bf:	0f 95 c0             	setne  al
 2c2:	20 c8                	and    al,cl
 2c4:	3c 01                	cmp    al,0x1
 2c6:	0f 85 db 00 00 00    	jne    3a7 <karatsuba_multiply+0x287>
# 	bi_isubtract(z3, z2);
 2cc:	4c 89 ef             	mov    rdi,r13
 2cf:	4c 89 f6             	mov    rsi,r14
 2d2:	e8 00 00 00 00       	call   2d7 <karatsuba_multiply+0x1b7>
			2d3: R_X86_64_PLT32	bi_isubtract-0x4
# 	bi_isubtract(z3, z0);
 2d7:	4c 89 ef             	mov    rdi,r13
 2da:	4c 89 fe             	mov    rsi,r15
 2dd:	e8 00 00 00 00       	call   2e2 <karatsuba_multiply+0x1c2>
			2de: R_X86_64_PLT32	bi_isubtract-0x4
# 	z3 = _bi_resize(z3, z3->len + i);
 2e2:	49 8b 75 00          	mov    rsi,QWORD PTR [r13+0x0]
 2e6:	48 d1 fe             	sar    rsi,1
 2e9:	48 01 ee             	add    rsi,rbp
 2ec:	4c 89 ef             	mov    rdi,r13
 2ef:	e8 00 00 00 00       	call   2f4 <karatsuba_multiply+0x1d4>
			2f0: R_X86_64_PLT32	_bi_resize-0x4
# 	if (!z3)
 2f4:	48 85 c0             	test   rax,rax
 2f7:	0f 84 f5 01 00 00    	je     4f2 <karatsuba_multiply+0x3d2>
 2fd:	49 89 c5             	mov    r13,rax
# 	z3->len -= i;
 300:	48 8b 00             	mov    rax,QWORD PTR [rax]
 303:	48 b9 fe ff ff ff ff 	movabs rcx,0x7ffffffffffffffe
 30a:	ff ff 7f
 30d:	48 8b 5c 24 38       	mov    rbx,QWORD PTR [rsp+0x38]
 312:	48 21 cb             	and    rbx,rcx
 315:	48 89 c1             	mov    rcx,rax
 318:	48 29 d9             	sub    rcx,rbx
 31b:	48 83 e1 fe          	and    rcx,0xfffffffffffffffe
 31f:	83 e0 01             	and    eax,0x1
 322:	48 09 c8             	or     rax,rcx
 325:	49 89 45 00          	mov    QWORD PTR [r13+0x0],rax
# 	bi_ishift_l(z3, i);
 329:	4c 89 ef             	mov    rdi,r13
 32c:	48 89 ee             	mov    rsi,rbp
 32f:	e8 00 00 00 00       	call   334 <karatsuba_multiply+0x214>
			330: R_X86_64_PLT32	bi_ishift_l-0x4
 334:	4c 89 f7             	mov    rdi,r14
# 	const len_ty z2_len = z2->len;
 337:	4d 8b 36             	mov    r14,QWORD PTR [r14]
 33a:	4c 89 f0             	mov    rax,r14
 33d:	48 d1 f8             	sar    rax,1
# 	result = _bi_resize(z2, max_of_3(z2_len + i * 2, z3->len, z0->len) + 1);
 340:	48 01 d8             	add    rax,rbx
 343:	49 8b 75 00          	mov    rsi,QWORD PTR [r13+0x0]
 347:	48 d1 fe             	sar    rsi,1
 34a:	49 8b 0f             	mov    rcx,QWORD PTR [r15]
 34d:	48 d1 f9             	sar    rcx,1
# 	if (b > maximum)
 350:	48 39 c6             	cmp    rsi,rax
 353:	48 0f 4e f0          	cmovle rsi,rax
# 	if (c > maximum)
 357:	48 39 f1             	cmp    rcx,rsi
 35a:	48 0f 4f f1          	cmovg  rsi,rcx
# 	result = _bi_resize(z2, max_of_3(z2_len + i * 2, z3->len, z0->len) + 1);
 35e:	48 ff c6             	inc    rsi
 361:	e8 00 00 00 00       	call   366 <karatsuba_multiply+0x246>
			362: R_X86_64_PLT32	_bi_resize-0x4
# 	if (!result)
 366:	48 85 c0             	test   rax,rax
 369:	74 3c                	je     3a7 <karatsuba_multiply+0x287>
# 	result->len = z2_len;
 36b:	48 8b 08             	mov    rcx,QWORD PTR [rax]
 36e:	49 83 e6 fe          	and    r14,0xfffffffffffffffe
 372:	83 e1 01             	and    ecx,0x1
 375:	4c 09 f1             	or     rcx,r14
 378:	48 89 08             	mov    QWORD PTR [rax],rcx
# 	bi_ishift_l(result, i * 2);
 37b:	48 89 c7             	mov    rdi,rax
 37e:	48 89 de             	mov    rsi,rbx
 381:	48 89 c3             	mov    rbx,rax
 384:	e8 00 00 00 00       	call   389 <karatsuba_multiply+0x269>
			385: R_X86_64_PLT32	bi_ishift_l-0x4
# 	bi_iadd(result, z3);
 389:	48 89 df             	mov    rdi,rbx
 38c:	4c 89 ee             	mov    rsi,r13
 38f:	e8 00 00 00 00       	call   394 <karatsuba_multiply+0x274>
			390: R_X86_64_PLT32	bi_iadd-0x4
 394:	49 89 de             	mov    r14,rbx
# 	bi_iadd(result, z0);
 397:	48 89 df             	mov    rdi,rbx
 39a:	4c 89 fe             	mov    rsi,r15
 39d:	e8 00 00 00 00       	call   3a2 <karatsuba_multiply+0x282>
			39e: R_X86_64_PLT32	bi_iadd-0x4
 3a2:	e9 51 01 00 00       	jmp    4f8 <karatsuba_multiply+0x3d8>
 3a7:	45 31 f6             	xor    r14d,r14d
 3aa:	e9 49 01 00 00       	jmp    4f8 <karatsuba_multiply+0x3d8>
 3af:	4c 8b 7c 24 08       	mov    r15,QWORD PTR [rsp+0x8]
# 	bigint *restrict product = _bi_resize(NULL, n1->len + n2->len);
 3b4:	49 8b 07             	mov    rax,QWORD PTR [r15]
 3b7:	48 d1 f8             	sar    rax,1
 3ba:	48 8b 33             	mov    rsi,QWORD PTR [rbx]
 3bd:	48 d1 fe             	sar    rsi,1
 3c0:	48 01 c6             	add    rsi,rax
 3c3:	31 ff                	xor    edi,edi
 3c5:	e8 00 00 00 00       	call   3ca <karatsuba_multiply+0x2aa>
			3c6: R_X86_64_PLT32	_bi_resize-0x4
 3ca:	49 89 c6             	mov    r14,rax
# 	bigint *const restrict current_mul = _bi_alloc(n1->len + n2->len);
 3cd:	49 8b 07             	mov    rax,QWORD PTR [r15]
 3d0:	48 d1 f8             	sar    rax,1
 3d3:	48 8b 3b             	mov    rdi,QWORD PTR [rbx]
 3d6:	48 d1 ff             	sar    rdi,1
 3d9:	48 01 c7             	add    rdi,rax
 3dc:	e8 00 00 00 00       	call   3e1 <karatsuba_multiply+0x2c1>
			3dd: R_X86_64_PLT32	_bi_alloc-0x4
 3e1:	49 89 c7             	mov    r15,rax
# 	if (!product || !current_mul)
 3e4:	4d 85 f6             	test   r14,r14
 3e7:	0f 95 c0             	setne  al
 3ea:	4d 85 ff             	test   r15,r15
 3ed:	0f 95 c1             	setne  cl
 3f0:	20 c1                	and    cl,al
 3f2:	80 f9 01             	cmp    cl,0x1
 3f5:	0f 85 27 01 00 00    	jne    522 <karatsuba_multiply+0x402>
# 	product->len = 1;
 3fb:	49 8b 06             	mov    rax,QWORD PTR [r14]
 3fe:	83 e0 01             	and    eax,0x1
 401:	48 83 c8 02          	or     rax,0x2
 405:	49 89 06             	mov    QWORD PTR [r14],rax
# 	for (n2_i = 0; n2_i < n2->len; ++n2_i)
 408:	48 83 3b 02          	cmp    QWORD PTR [rbx],0x2
 40c:	0f 8c 1b 01 00 00    	jl     52d <karatsuba_multiply+0x40d>
 412:	48 89 d8             	mov    rax,rbx
 415:	49 8b 77 08          	mov    rsi,QWORD PTR [r15+0x8]
 419:	45 31 ed             	xor    r13d,r13d
 41c:	48 bd b3 94 d6 26 e8 	movabs rbp,0x112e0be826d694b3
 423:	0b 2e 11
 426:	45 31 e4             	xor    r12d,r12d
 429:	48 8b 7c 24 08       	mov    rdi,QWORD PTR [rsp+0x8]
 42e:	eb 42                	jmp    472 <karatsuba_multiply+0x352>
 430:	45 31 c9             	xor    r9d,r9d
 433:	31 d2                	xor    edx,edx
# 		current_mul->num[n2_i + n1_i] = byt_prod;
 435:	4a 8d 04 a6          	lea    rax,[rsi+r12*4]
 439:	42 89 14 88          	mov    DWORD PTR [rax+r9*4],edx
# 		bi_iadd(product, current_mul);
 43d:	4c 89 f7             	mov    rdi,r14
 440:	4c 89 fe             	mov    rsi,r15
 443:	e8 00 00 00 00       	call   448 <karatsuba_multiply+0x328>
			444: R_X86_64_PLT32	bi_iadd-0x4
 448:	49 8b 77 08          	mov    rsi,QWORD PTR [r15+0x8]
 44c:	48 89 d8             	mov    rax,rbx
 44f:	48 8b 7c 24 08       	mov    rdi,QWORD PTR [rsp+0x8]
 454:	42 c7 04 a6 00 00 00 	mov    DWORD PTR [rsi+r12*4],0x0
 45b:	00
# 	for (n2_i = 0; n2_i < n2->len; ++n2_i)
 45c:	49 ff c4             	inc    r12
 45f:	48 8b 08             	mov    rcx,QWORD PTR [rax]
 462:	48 d1 f9             	sar    rcx,1
 465:	49 83 c5 04          	add    r13,0x4
 469:	49 39 cc             	cmp    r12,rcx
 46c:	0f 8d bb 00 00 00    	jge    52d <karatsuba_multiply+0x40d>
# 		if (n2->num[n2_i] == 0)
 472:	4c 8b 40 08          	mov    r8,QWORD PTR [rax+0x8]
 476:	43 83 3c a0 00       	cmp    DWORD PTR [r8+r12*4],0x0
 47b:	74 d7                	je     454 <karatsuba_multiply+0x334>
# 		current_mul->len = n1->len + (n2_i + 1);
 47d:	48 8b 07             	mov    rax,QWORD PTR [rdi]
 480:	49 8b 0f             	mov    rcx,QWORD PTR [r15]
 483:	4a 8d 14 60          	lea    rdx,[rax+r12*2]
 487:	48 83 c2 02          	add    rdx,0x2
 48b:	48 83 e2 fe          	and    rdx,0xfffffffffffffffe
 48f:	83 e1 01             	and    ecx,0x1
 492:	48 09 d1             	or     rcx,rdx
 495:	49 89 0f             	mov    QWORD PTR [r15],rcx
# 		for (n1_i = 0; n1_i < n1->len; ++n1_i)
 498:	48 83 f8 02          	cmp    rax,0x2
 49c:	7c 92                	jl     430 <karatsuba_multiply+0x310>
 49e:	4c 8b 57 08          	mov    r10,QWORD PTR [rdi+0x8]
 4a2:	4e 8d 1c 2e          	lea    r11,[rsi+r13*1]
 4a6:	31 d2                	xor    edx,edx
 4a8:	45 31 c9             	xor    r9d,r9d
 4ab:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
# 			byt_prod += (ldigit_ty)n2->num[n2_i] * n1->num[n1_i];
 4b0:	4b 63 04 a0          	movsxd rax,DWORD PTR [r8+r12*4]
 4b4:	4b 63 0c 8a          	movsxd rcx,DWORD PTR [r10+r9*4]
 4b8:	48 0f af c8          	imul   rcx,rax
 4bc:	48 01 d1             	add    rcx,rdx
# 			byt_prod /= BIGINT_BASE;
 4bf:	48 89 c8             	mov    rax,rcx
 4c2:	48 f7 ed             	imul   rbp
 4c5:	48 89 d0             	mov    rax,rdx
 4c8:	48 c1 e8 3f          	shr    rax,0x3f
 4cc:	48 c1 fa 1a          	sar    rdx,0x1a
 4d0:	48 01 c2             	add    rdx,rax
 4d3:	69 c2 00 ca 9a 3b    	imul   eax,edx,0x3b9aca00
 4d9:	29 c1                	sub    ecx,eax
# 			current_mul->num[n2_i + n1_i] = byt_prod % BIGINT_BASE;
 4db:	43 89 0c 8b          	mov    DWORD PTR [r11+r9*4],ecx
# 		for (n1_i = 0; n1_i < n1->len; ++n1_i)
 4df:	49 ff c1             	inc    r9
 4e2:	48 8b 07             	mov    rax,QWORD PTR [rdi]
 4e5:	48 d1 f8             	sar    rax,1
 4e8:	49 39 c1             	cmp    r9,rax
 4eb:	7c c3                	jl     4b0 <karatsuba_multiply+0x390>
 4ed:	e9 43 ff ff ff       	jmp    435 <karatsuba_multiply+0x315>
 4f2:	45 31 f6             	xor    r14d,r14d
 4f5:	45 31 ed             	xor    r13d,r13d
 4f8:	48 8b 7c 24 08       	mov    rdi,QWORD PTR [rsp+0x8]
# 	_bi_free(xhi_plus_xlo);
 4fd:	e8 00 00 00 00       	call   502 <karatsuba_multiply+0x3e2>
			4fe: R_X86_64_PLT32	_bi_free-0x4
# 	_bi_free(yhi_plus_ylo);
 502:	4c 89 e7             	mov    rdi,r12
 505:	e8 00 00 00 00       	call   50a <karatsuba_multiply+0x3ea>
			506: R_X86_64_PLT32	_bi_free-0x4
# 	_bi_free(z0);
 50a:	4c 89 ff             	mov    rdi,r15
 50d:	e8 00 00 00 00       	call   512 <karatsuba_multiply+0x3f2>
			50e: R_X86_64_PLT32	_bi_free-0x4
# 	_bi_free(z3);
 512:	4c 89 ef             	mov    rdi,r13
 515:	e8 00 00 00 00       	call   51a <karatsuba_multiply+0x3fa>
			516: R_X86_64_PLT32	_bi_free-0x4
 51a:	4c 89 f0             	mov    rax,r14
 51d:	e9 8e fc ff ff       	jmp    1b0 <karatsuba_multiply+0x90>
# 		product = _bi_free(product);
 522:	4c 89 f7             	mov    rdi,r14
 525:	e8 00 00 00 00       	call   52a <karatsuba_multiply+0x40a>
			526: R_X86_64_PLT32	_bi_free-0x4
 52a:	49 89 c6             	mov    r14,rax
# 	_bi_free(current_mul);
 52d:	4c 89 ff             	mov    rdi,r15
 530:	e8 00 00 00 00       	call   535 <karatsuba_multiply+0x415>
			531: R_X86_64_PLT32	_bi_free-0x4
# 		return (_bi_trim(long_multiply(n1, n2)));
 535:	4c 89 f7             	mov    rdi,r14
 538:	48 83 c4 68          	add    rsp,0x68
 53c:	5b                   	pop    rbx
 53d:	41 5c                	pop    r12
 53f:	41 5d                	pop    r13
 541:	41 5e                	pop    r14
 543:	41 5f                	pop    r15
 545:	5d                   	pop    rbp
 546:	e9 00 00 00 00       	jmp    54b <karatsuba_multiply+0x42b>
			547: R_X86_64_PLT32	_bi_trim-0x4
