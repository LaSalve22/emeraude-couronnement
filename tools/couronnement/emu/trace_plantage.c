#include <mgba/core/core.h>
#include <mgba/core/log.h>
#include <mgba/gba/core.h>
#include <mgba/internal/arm/arm.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
static void nolog(struct mLogger*l,int c,enum mLogLevel lv,const char*f,va_list a){}
static struct mLogger L={.log=nolog};
int main(int argc,char**argv){ mLogSetDefaultLogger(&L);
  struct mCore*core=GBACoreCreate(); core->init(core); mCoreInitConfig(core,NULL);
  unsigned w,h; core->desiredVideoDimensions(core,&w,&h);
  color_t*buf=malloc(w*h*sizeof(color_t)); core->setVideoBuffer(core,buf,w);
  mCoreLoadFile(core,argv[1]); core->reset(core);
  FILE*f=fopen(argv[2],"r"); int n; unsigned k; char line[256];
  while(fgets(line,sizeof line,f)){ if(sscanf(line,"%d %x",&n,&k)<2) continue; core->setKeys(core,k); for(int i=0;i<n;i++) core->runFrame(core);}
  struct ARMCore*cpu=core->cpu; unsigned ring[4096]; int ri=0; unsigned lrs[4096];
  core->setKeys(core,1);
  for(long s=0;s<60L*300000;s++){ if(s==5*280000) core->setKeys(core,0);
    unsigned pc=cpu->gprs[15];
    if(pc>=0x08000000 && pc<0x0A000000){ ring[ri&4095]=pc; lrs[ri&4095]=cpu->gprs[14]; ri++; }
    if(pc>=0x08000000 && pc<0x080000F0 && ri>10){ for(int i=0;i<16;i++){unsigned b=0x02001f6c+i*0x24; if(core->busRead8(core,b)&1) printf("obj%d gfx=%d local=%d x=%d y=%d\n",i,core->busRead16(core,b+4),core->busRead8(core,b+8),(short)core->busRead16(core,b+0x10),(short)core->busRead16(core,b+0x12));} for(int i=0;i<64;i++){unsigned b=0x020369f4+i*0x44; unsigned fl=core->busRead16(core,b+0x3E); if(fl&1) printf("spr%d img=%08x anims=%08x cb=%08x tpl=%08x animNum=%d cmd=%d\n",i,core->busRead32(core,b+0xC),core->busRead32(core,b+8),core->busRead32(core,b+0x1C),core->busRead32(core,b+0x14),core->busRead8(core,b+0x2A),core->busRead8(core,b+0x2B));} printf("reset at step %ld\n",s); for(int j=ri-300;j<ri;j++) printf("%08x lr=%08x\n",ring[j&4095],lrs[j&4095]); return 0;}
    core->step(core);}
  printf("no reset\n"); return 0;}
