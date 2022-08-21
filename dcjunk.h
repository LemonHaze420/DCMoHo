#ifndef DCJUNK_H
#define DCJUNK_H

typedef struct 
{
 float x,y,z;
 float iv[3];
 float jv[3];
 float kv[3];
 } viewframe;
 
typedef struct {
float x,y,z;
} tvector; 

VECTOR *FApplyMatrix( FMatrix * m, SVECTOR * v0, VECTOR * v1 );
FMatrix *FSetMulRotMatrix(FMatrix *m0);
long FRotTransPers(FVector *v0,float *sxy,long *p,long *flag);
SVECTOR *FTransMultPoints(FVector *from,FVector *to,int count); 
void FSetRotMatrix(FMatrix *m); 
void FSetTransMatrix(FMatrix *m);
void FSetGeomOffset(long ofx,long ofy);
FMatrix *FTransMatrix(FMatrix *m,VECTOR *v);
void FMatrixToViewFrame(FMatrix *in,viewframe *out);
void resetViewframe(viewframe *vf);
void InitViewFrame(viewframe *vf);
void regen_cosinesK(viewframe &vf);
void PersTrans(int vn,viewframe vfCam, viewframe vf,tvector *in,tvector *out, SINT stride = 1);
void PersTransWrapper(int mVertexCount,tvector *in,tvector *out,const FVector LocalPos,const FMatrix LocalOrientation, SINT stride = 1);
void PersTrans2(int vn,viewframe vfCam, viewframe vf,tvector *in,tvector *out);
void PersTrans3(int vn,int inc,viewframe vfCam, viewframe vf,tvector *in,tvector *out);
void PersTransWrapper2(int mVertexCount,tvector *in,tvector *out,const FVector LocalPos,const FMatrix LocalOrientation);
void PersTransWrapper3(int mVertexCount,int inc,tvector *in,tvector *out,const FVector LocalPos,const FMatrix LocalOrientation);

void rot_vx(float rotAng,viewframe *vf);
void rot_vy(float rotAng,viewframe *vf);
void rot_vz(float rotAng,viewframe &vf);
#endif

