/*$Id: f90_common.c,v 1.8 2001/03/23 23:20:56 balay Exp $*/

#include "petscf90.h"

#if defined PETSC_HAVE_F90_C
#include PETSC_HAVE_F90_C

/* Check if PETSC_HAVE_F90_H is also specified */
#if !defined(PETSC_HAVE_F90_H)
#error "Both PETSC_HAVE_F90_H and PETSC_HAVE_F90_C flags have to be specified in petscconf.h"
#endif

/* Nag uses char * instead of void* ??? */
#if !defined(__F90_NAG_H)
#define Pointer void*
#endif
/*-------------------------------------------------------------*/
#undef __FUNCT__  
#define __FUNCT__ "F90Array1dAccess"
int F90Array1dAccess(F90Array1d *ptr,void **array)
{
  PetscFunctionBegin;
  PetscValidPointer(array);
  PetscValidPointer(ptr);
  *array = ptr->addr;
  PetscFunctionReturn(0);
}

#undef __FUNCT__  
#define __FUNCT__ "F90Array1dDestroy"
int F90Array1dDestroy(F90Array1d *ptr)
{
  PetscFunctionBegin;
  PetscValidPointer(ptr);
  ptr->addr = (Pointer)0;
  PetscFunctionReturn(0);
}
#undef __FUNCT__  
#define __FUNCT__ "F90Array1dGetNextRecord"
int F90Array1dGetNextRecord(F90Array1d *ptr,void **next)
{
  PetscFunctionBegin;
  PetscValidPointer(ptr);
  *next = (void*)(ptr + 1);
  PetscFunctionReturn(0);
}

/*-------------------------------------------------------------*/

#undef __FUNCT__  
#define __FUNCT__ "F90Array2dAccess"
int F90Array2dAccess(F90Array2d *ptr,void **array)
{
  PetscFunctionBegin;
  PetscValidPointer(array);
  PetscValidPointer(ptr);
  *array = ptr->addr;
  PetscFunctionReturn(0);
}

#undef __FUNCT__  
#define __FUNCT__ "F90Array2dDestroy"
int F90Array2dDestroy(F90Array2d *ptr)
{
  PetscFunctionBegin;
  PetscValidPointer(ptr);
  ptr->addr = (Pointer)0;
  PetscFunctionReturn(0);
}
#undef __FUNCT__  
#define __FUNCT__ "F90Array1dGetNextRecord"
int F90Array2dGetNextRecord(F90Array2d *ptr,void **next)
{
  PetscFunctionBegin;
  PetscValidPointer(ptr);
  *next = (void*)(ptr + 1);
  PetscFunctionReturn(0);
}
/*-------------------------------------------------------------*/

#endif
