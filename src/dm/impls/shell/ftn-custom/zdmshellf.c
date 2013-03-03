#include <petsc-private/fortranimpl.h>
#include <petscdmshell.h>       /*I    "petscdmshell.h"  I*/

#if defined(PETSC_HAVE_FORTRAN_CAPS)
#define dmshellsetcreatematrix_                DMSHELLSETCREATEMATRIX
#define dmshellsetcreateglobalvector_          DMSHELLSETCREATEGLOBALVECTOR_
#define dmshellsetcreatelocalvector_           DMSHELLSETCREATELOCALVECTOR_
#define dmshellsetglobaltolocal_               DMSHELLSETGLOBALTOLOCAL_
#define dmshellsetlocaltoglobal_               DMSHELLSETLOCALTOGLOBAL_
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE)
#define dmshellsetcreatematrix_                dmshellsetcreatematrix
#define dmshellsetcreateglobalvector_          dmshellsetcreateglobalvector
#define dmshellsetcreatelocalvector_           dmshellsetcreatelocalvector
#define dmshellsetglobaltolocal_               dmshellsetglobaltolocal
#define dmshellsetlocaltoglobal_               dmshellsetlocaltoglobal
#endif

/*
 * C routines are required for matrix and global vector creation.  We define C routines here that call the corresponding
 * Fortran routine (indexed by _cb) that was set by the user.
 */

static struct {
  PetscFortranCallbackId creatematrix;
  PetscFortranCallbackId createglobalvector;
  PetscFortranCallbackId createlocalvector;
  PetscFortranCallbackId globaltolocalbegin;
  PetscFortranCallbackId globaltolocalend;
  PetscFortranCallbackId localtoglobalbegin;
  PetscFortranCallbackId localtoglobalend;
} _cb;

#undef __FUNCT__
#define __FUNCT__ "ourcreatematrix"
static PetscErrorCode ourcreatematrix(DM dm,MatType type,Mat *A)
{
  int  len;
  char *ftype = (char*)type;
  if (type) {
    size_t slen;
    PetscStrlen(type,&slen);
    len = (int)slen;
  } else {
    type = PETSC_NULL_CHARACTER_Fortran;
    len  = 0;
  }
  PetscObjectUseFortranCallbackSubType(dm,_cb.creatematrix,(DM*,CHAR PETSC_MIXED_LEN_PROTO,Mat*,PetscErrorCode* PETSC_END_LEN_PROTO),
                                       (&dm,ftype PETSC_MIXED_LEN_CALL(len),A,&ierr PETSC_END_LEN_CALL(len)));
  return 0;
}

#undef __FUNCT__
#define __FUNCT__ "ourcreateglobalvector"
static PetscErrorCode ourcreateglobalvector(DM dm,Vec *v)
{
  PetscObjectUseFortranCallbackSubType(dm,_cb.createglobalvector,(DM*,Vec*,PetscErrorCode*),(&dm,v,&ierr));
  return 0;
}

#undef __FUNCT__
#define __FUNCT__ "ourcreatelocalvector"
static PetscErrorCode ourcreatelocalvector(DM dm,Vec *v)
{
  PetscObjectUseFortranCallbackSubType(dm,_cb.createlocalvector,(DM*,Vec*,PetscErrorCode*),(&dm,v,&ierr));
  return 0;
}

#undef __FUNCT__
#define __FUNCT__ "ourglobaltolocalbegin"
static PetscErrorCode ourglobaltolocalbegin(DM dm,Vec g,InsertMode mode,Vec l)
{
  PetscObjectUseFortranCallbackSubType(dm,_cb.globaltolocalbegin,(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),(&dm,&g,&mode,&l,&ierr));
  return 0;
}

#undef __FUNCT__
#define __FUNCT__ "ourglobaltolocalend"
static PetscErrorCode ourglobaltolocalend(DM dm,Vec g,InsertMode mode,Vec l)
{
  PetscObjectUseFortranCallbackSubType(dm,_cb.globaltolocalend,(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),(&dm,&g,&mode,&l,&ierr));
  return 0;
}

#undef __FUNCT__
#define __FUNCT__ "ourlocaltoglobalbegin"
static PetscErrorCode ourlocaltoglobalbegin(DM dm,Vec l,InsertMode mode,Vec g)
{
  PetscObjectUseFortranCallbackSubType(dm,_cb.localtoglobalbegin,(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),(&dm,&l,&mode,&g,&ierr));
  return 0;
}

#undef __FUNCT__
#define __FUNCT__ "ourlocaltoglobalend"
static PetscErrorCode ourlocaltoglobalend(DM dm,Vec l,InsertMode mode,Vec g)
{
  PetscObjectUseFortranCallbackSubType(dm,_cb.localtoglobalend,(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),(&dm,&l,&mode,&g,&ierr));
  return 0;
}

PETSC_EXTERN_C void PETSC_STDCALL dmshellsetcreatematrix_(DM *dm,void (PETSC_STDCALL *func)(DM*,CHAR type PETSC_MIXED_LEN(len),Mat*,PetscErrorCode* PETSC_END_LEN(len)),PetscErrorCode *ierr)
{
  *ierr = PetscObjectSetFortranCallback((PetscObject)*dm,PETSC_FORTRAN_CALLBACK_SUBTYPE,&_cb.creatematrix,(PetscVoidFunction)func,NULL);
  if (*ierr) return;
  *ierr = DMShellSetCreateMatrix(*dm,ourcreatematrix);
}

PETSC_EXTERN_C void PETSC_STDCALL dmshellsetcreateglobalvector_(DM *dm,void (PETSC_STDCALL *func)(DM*,Vec*,PetscErrorCode*),PetscErrorCode *ierr)
{
  *ierr = PetscObjectSetFortranCallback((PetscObject)*dm,PETSC_FORTRAN_CALLBACK_SUBTYPE,&_cb.createglobalvector,(PetscVoidFunction)func,NULL);
  if (*ierr) return;
  *ierr = DMShellSetCreateGlobalVector(*dm,ourcreateglobalvector);
}

PETSC_EXTERN_C void PETSC_STDCALL dmshellsetcreatelocalvector_(DM *dm,void (PETSC_STDCALL *func)(DM*,Vec*,PetscErrorCode*),PetscErrorCode *ierr)
{
  *ierr = PetscObjectSetFortranCallback((PetscObject)*dm,PETSC_FORTRAN_CALLBACK_SUBTYPE,&_cb.createlocalvector,(PetscVoidFunction)func,NULL);
  if (*ierr) return;
  *ierr = DMShellSetCreateLocalVector(*dm,ourcreatelocalvector);
}

PETSC_EXTERN_C void PETSC_STDCALL dmshellsetglobaltolocal_(DM *dm,void (PETSC_STDCALL *begin)(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),void (PETSC_STDCALL *end)(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),PetscErrorCode *ierr)
{
  *ierr = PetscObjectSetFortranCallback((PetscObject)*dm,PETSC_FORTRAN_CALLBACK_SUBTYPE,&_cb.globaltolocalbegin,(PetscVoidFunction)begin,NULL);
  if (*ierr) return;
  *ierr = PetscObjectSetFortranCallback((PetscObject)*dm,PETSC_FORTRAN_CALLBACK_SUBTYPE,&_cb.globaltolocalend,(PetscVoidFunction)end,NULL);
  if (*ierr) return;
  *ierr = DMShellSetGlobalToLocal(*dm,ourglobaltolocalbegin,ourglobaltolocalend);
}

PETSC_EXTERN_C void PETSC_STDCALL dmshellsetlocaltoglobal_(DM *dm,void (PETSC_STDCALL *begin)(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),void (PETSC_STDCALL *end)(DM*,Vec*,InsertMode*,Vec*,PetscErrorCode*),PetscErrorCode *ierr)
{
  *ierr = PetscObjectSetFortranCallback((PetscObject)*dm,PETSC_FORTRAN_CALLBACK_SUBTYPE,&_cb.localtoglobalbegin,(PetscVoidFunction)begin,NULL);
  if (*ierr) return;
  *ierr = PetscObjectSetFortranCallback((PetscObject)*dm,PETSC_FORTRAN_CALLBACK_SUBTYPE,&_cb.localtoglobalend,(PetscVoidFunction)end,NULL);
  if (*ierr) return;
  *ierr = DMShellSetLocalToGlobal(*dm,ourlocaltoglobalbegin,ourlocaltoglobalend);
}
