
#ifndef _HOE_EDITOR_UTILS_H_
#define _HOE_EDITOR_UTILS_H_

namespace HoeEditor {

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) if (ptr) delete (ptr);ptr = NULL;
#endif

wxString FullPath(const wxString &dir, const wxString &relpath);

}; // namespace HoeEditor


#endif // _HOE_EDITOR_UTILS_H_

