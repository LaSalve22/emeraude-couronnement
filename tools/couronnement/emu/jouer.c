#include <mgba/core/core.h>
#include <mgba/core/blip_buf.h>
#include <mgba/gba/core.h>
#include <mgba-util/vfs.h>
#include <mgba/core/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// usage: h rom script outprefix ; script lines: "<frames> <keys hex> [shot]"
static void nolog(struct mLogger*l,int c,enum mLogLevel lv,const char*f,va_list a){}
static struct mLogger L={.log=nolog};
int main(int argc,char**argv){ mLogSetDefaultLogger(&L);
  struct mCore*core=GBACoreCreate(); core->init(core); mCoreInitConfig(core,NULL);
  unsigned w,h; core->desiredVideoDimensions(core,&w,&h);
  color_t*buf=malloc(w*h*sizeof(color_t)); core->setVideoBuffer(core,buf,w);
  mCoreLoadFile(core,argv[1]); core->reset(core);
  FILE*f=fopen(argv[2],"r"); int n; unsigned k; char tag[64]; int shot=0; char line[256];
  while(fgets(line,sizeof line,f)){
    tag[0]=0; if(sscanf(line,"%d %x %63s",&n,&k,tag)<2) continue;
    core->setKeys(core,k);
    for(int i=0;i<n;i++) core->runFrame(core);
    if(tag[0]){ char fn[256]; sprintf(fn,"%s_%02d_%s.ppm",argv[3],shot++,tag); FILE*o=fopen(fn,"wb");
      fprintf(o,"P6 %u %u 255\n",w,h);
      for(unsigned i=0;i<w*h;i++){ color_t c=buf[i]; unsigned char p[3]={c&0xFF,(c>>8)&0xFF,(c>>16)&0xFF}; fwrite(p,1,3,o);} fclose(o);}
  }
  printf("pc=%08x\n", 0); core->deinit(core); return 0;}
