/*-----------------09/09/1999 13:02-----------------
 * This is the header fiole associated with the assembly
 * file 3DENGINE.ASM.
 * --------------------------------------------------*/

#ifdef JCLDCNOPSX		// removed...

extern "C" SVECTOR *TransMultPoints(SVECTOR *from,SVECTOR*to,int count);  // Function in 3DENGINE.ASM
extern "C" SVECTOR *TransMultNormal_Light(SVECTOR *from,SVECTOR*to,int count);  // Function in 3DENGINE.ASM
extern "C" POLY_GT3 *MakePolysFromList(POLY_GT3 *p3,SDCMeshGTriangle *tri,SVECTOR*vc,long *vn,int count,unsigned long *o, SDCMeshTTriangle *ttri);//,long Mask,long GouraudDepth);

#endif