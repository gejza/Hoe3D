
#include <stdio.h>
#include <memory.h>

#include "../../Hoe3D/src/hoe_format.h"
#include "../../Hoe3D/include/hfmt/hres_file.h"
#include "../../Hoe3D/include/hfmt/htex_file.h"
#include "linker.h"
#include "compiler.h"
#include "texbase.h"

void HTexLinker::Add(HTexBase * tex)
{
        texs.push_back(tex);
}

bool HTexLinker::Build(HoeUtils::Stream * stream)
{
        if (texs.size() == 0)
                return false;

        hfres_header head = {IDRESHEADER, IDTEXHEADER, IDTEXVER,(int)texs.size(),0};
        stream->Write(&head,sizeof(hfres_header));

        int namespos = stream->GetPos();
        hfres_name * names = new hfres_name[texs.size()];
        memset(names,0,sizeof(hfres_name)*texs.size());
        stream->Write(names,sizeof(hfres_name)*texs.size());

        // ulozeni jednotlivejch textur
        for (unsigned int i=0;i < texs.size();i++)
        {
                // export textury
                HTexBase * tex = texs[i];
                const char * name = tex->GetName();
                Progress((int)texs.size(),i,name);
                if (strlen(name) > (32-2))
                {
					fprintf(stderr,"WARNING: texture name %s sized\n", name);
                        //delete names;
                        //return false;
					strncpy(names[i].name,name,31);
					names[i].name[31] = 0;
                }
				else
				{
					strcpy(names[i].name,name);
				}
                names[i].filepos = stream->GetPos();

                HTexCompiler * cc = tex->GetCompiler();
                if (cc == NULL || !cc->SaveToStream(stream))
                {
                        delete names;
                        if (cc) cc->Release();
                        return false;
                }

                cc->Release();
        }

        stream->SetPos(namespos);
        stream->Write(names,sizeof(hfres_name)*texs.size());
        delete names;

        Progress((int)texs.size(),0,NULL);

        return true;
}


bool HTexLinker::Progress(int count, int cur, const char * file)
{
        return true;
}
