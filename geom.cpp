// Urrgghhhh gotta convert this to lovely C++
//*******************************************




void normalise(float a, float b,float c)
{
float  nf:=sqrt((A*A)+(B*B)+(C*C));

   a=a/nf;
   b=b/nf;
   c=c/nf;
}

void resetViewframe(viewframe *vf)
{
vf.iv[0]=1;  // x
vf.iv[1]=0;
vf.iv[2]=0;

vf.jv[0]=0;  // y - down
vf.jv[1]=1;
vf.jv[2]=0;

vf.kv[0]=0;  // z -
vf.kv[1]=0;
vf.kv[2]=1;
}

void initViewframe(viewframe *vf)
{
vf.zoom=1;
vf.sx=1;
vf.sy=1;
vf.sz=1;

vf.iv[0]=1;  // x
vf.iv[1]=0;
vf.iv[2]=0;

vf.jv[0]=0;  // y - down
vf.jv[1]=1;
vf.jv[2]=0;

vf.kv[0]=0;  // z -
vf.kv[1]=0;
vf.kv[2]=1;
//rot_vx(2,vf);
}


void nrm_vec(float n1, float n2, float n3)
{
 float mag_vec=1.0/sqrt((n1*n1) + (n2*n2) + (n3*n3));
 n1=n1*mag_vec;
 n2=n2*mag_vec;
 n3=n3*mag_vec;
}

void  AxB( vector n,vector v1,vector v2)
{

float d0,d1,d2;

d0=(v1[1]*v2[2])-(v1[2]*v2[1]);
d1=(v1[2]*v2[0])-(v1[0]*v2[2]);
d2=(v1[0]*v2[1])-(v1[1]*v2[0]);

	//normalise the components
nrm_vec(d0,d1,d2);

n[0]=d0;
n[1]=d1;
n[2]=d2;
}

void regen_cosinesK(viewframe *vf)
{
 //kv is normalised
	nrm_vec(vf.kv[0],vf.kv[1],vf.kv[2]);
	AxB(vf.iv,vf.jv,vf.kv);
	AxB(vf.jv,vf.kv,vf.iv);
}

void regen_cosinesI(viewframe *vf)
{
 nrm_vec(vf.iv[0],vf.iv[1],vf.iv[2]);
 AxB(vf.jv,vf.kv,vf.iv);
 AxB(vf.kv,vf.iv,vf.jv);
}


void regen_cosinesJ(viewframe *vf)
{
 nrm_vec(vf.jv[0],vf.jv[1],vf.jv[2]);
 AxB(vf.kv,vf.iv,vf.jv);
 AxB(vf.iv,vf.jv,vf.kv);
}

void regen_cosinesJI(viewframe *vf)
{
 nrm_vec(vf.jv[0],vf.jv[1],vf.jv[2]);
 AxB(vf.iv,vf.jv,vf.kv);
 AxB(vf.kv,vf.iv,vf.jv);
}




void PersTrans(int vn,viewframe vfCam, viewframe VF,void *zx,void *transxyz)
{
float n11,n12,n13,n21,n22,n23,n31,n32,n33;
float c11,c12,c13,c21,c22,c23,c31,c32,c33;
float f11,f12,f13,f21,f22,f23,f31,f32,f33;
float xadd,yadd,zadd;
int lastvertex,pt;
float xw,yw,zw;
float xv,yv,zv;

zz=zx;
LastVertex:=0;
n11:=vf.iv[0];
n12:=vf.iv[1];
n13:=vf.iv[2];
n21:=vf.jv[0];
n22:=vf.jv[1];
n23:=vf.jv[2];
n31:=vf.kv[0];
n32:=vf.kv[1];
n33:=vf.kv[2];
c11:=vfCam.iv[0];
c12:=vfCam.iv[1];
c13:=vfCam.iv[2];
c21:=vfCam.jv[0];
c22:=vfCam.jv[1];
c23:=vfCam.jv[2];
c31:=vfCam.kv[0];
c32:=vfCam.kv[1];
c33:=vfCam.kv[2];

f11:=(c11*n11) + (c12*n12) + (c13*n13);
f12:=(c11*n21) + (c12*n22) + (c13*n23);
f13:=(c11*n31) + (c12*n32) + (c13*n33);
xAdd:=((vf.x-vfCam.x)*c11) + ((vf.y-vfCam.y)*c12) + ((vf.z-vfCam.z)*c13);

f21:=(c21*n11) + (c22*n12) + (c23*n13);
f22:=(c21*n21) + (c22*n22) + (c23*n23);
f23:=(c21*n31) + (c22*n32) + (c23*n33);
yAdd:=((vf.x-vfCam.x)*c21) + ((vf.y-vfCam.y)*c22) + ((vf.z-vfCam.z)*c23);

f31:=(c31*n11) + (c32*n12) + (c33*n13);
f32:=(c31*n21) + (c32*n22) + (c33*n23);
f33:=(c31*n31) + (c32*n32) + (c33*n33);
zAdd:=((vf.x-vfCam.x)*c31) + ((vf.y-vfCam.y)*c32) + ((vf.z-vfCam.z)*c33);

for (int pt=0;pt<vn;pt++)
 {
   xW=zz[pt].x;
   yW=zz[pt].y;
   zW=zz[pt].z;
   xV=(xW*f11)  + (yW*f12) + (zW*f13)+xAdd;
   yV=(xW*f21)  + (yW*f22) + (zW*f23)+yAdd;
   zV=(xW*f31)  + (yW*f32) + (zW*f33)+zAdd;
   transxyz[pt].x=xv;
   transxyz[pt].y=yv;
   transxyz[pt].z=zv;
  }
}
