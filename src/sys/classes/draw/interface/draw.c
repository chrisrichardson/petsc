
/*
       Provides the calling sequences for all the basic PetscDraw routines.
*/
#include <petsc/private/drawimpl.h>  /*I "petscdraw.h" I*/
#include <petscviewer.h>

PetscClassId PETSC_DRAW_CLASSID;

static PetscBool PetscDrawPackageInitialized = PETSC_FALSE;
#undef __FUNCT__
#define __FUNCT__ "PetscDrawFinalizePackage"
/*@C
  PetscDrawFinalizePackage - This function destroys everything in the Petsc interface to the Draw package. It is
  called from PetscFinalize().

  Level: developer

.keywords: Petsc, destroy, package, mathematica
.seealso: PetscFinalize()
@*/
PetscErrorCode  PetscDrawFinalizePackage(void)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFunctionListDestroy(&PetscDrawList);CHKERRQ(ierr);
  PetscDrawPackageInitialized = PETSC_FALSE;
  PetscDrawRegisterAllCalled  = PETSC_FALSE;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawInitializePackage"
/*@C
  PetscInitializeDrawPackage - This function initializes everything in the PetscDraw package. It is called
  from PetscDLLibraryRegister() when using dynamic libraries, and on the call to PetscInitialize()
  when using static libraries.

  Level: developer

.keywords: Petsc, initialize, package
.seealso: PetscInitialize()
@*/
PetscErrorCode  PetscDrawInitializePackage(void)
{
  char           logList[256];
  char           *className;
  PetscBool      opt;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (PetscDrawPackageInitialized) PetscFunctionReturn(0);
  PetscDrawPackageInitialized = PETSC_TRUE;
  /* Register Classes */
  ierr = PetscClassIdRegister("Draw",&PETSC_DRAW_CLASSID);CHKERRQ(ierr);
  ierr = PetscClassIdRegister("Axis",&PETSC_DRAWAXIS_CLASSID);CHKERRQ(ierr);
  ierr = PetscClassIdRegister("Line Graph",&PETSC_DRAWLG_CLASSID);CHKERRQ(ierr);
  ierr = PetscClassIdRegister("Histogram",&PETSC_DRAWHG_CLASSID);CHKERRQ(ierr);
  ierr = PetscClassIdRegister("Bar Grap",&PETSC_DRAWBAR_CLASSID);CHKERRQ(ierr);
  ierr = PetscClassIdRegister("Scatter Plot",&PETSC_DRAWSP_CLASSID);CHKERRQ(ierr);
  /* Register Constructors */
  ierr = PetscDrawRegisterAll();CHKERRQ(ierr);
  /* Process info exclusions */
  ierr = PetscOptionsGetString(NULL,NULL, "-info_exclude", logList, 256, &opt);CHKERRQ(ierr);
  if (opt) {
    ierr = PetscStrstr(logList, "draw", &className);CHKERRQ(ierr);
    if (className) {
      ierr = PetscInfoDeactivateClass(0);CHKERRQ(ierr);
    }
  }
  /* Process summary exclusions */
  ierr = PetscOptionsGetString(NULL,NULL, "-log_summary_exclude", logList, 256, &opt);CHKERRQ(ierr);
  if (opt) {
    ierr = PetscStrstr(logList, "draw", &className);CHKERRQ(ierr);
    if (className) {
      ierr = PetscLogEventDeactivateClass(0);CHKERRQ(ierr);
    }
  }
  ierr = PetscRegisterFinalize(PetscDrawFinalizePackage);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawResizeWindow"
/*@
   PetscDrawResizeWindow - Allows one to resize a window from a program.

   Collective on PetscDraw

   Input Parameter:
+  draw - the window
-  w,h - the new width and height of the window

   Level: intermediate

.seealso: PetscDrawCheckResizedWindow()
@*/
PetscErrorCode  PetscDrawResizeWindow(PetscDraw draw,int w,int h)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  PetscValidLogicalCollectiveInt(draw,w,2);
  PetscValidLogicalCollectiveInt(draw,h,3);
  if (draw->ops->resizewindow) {
    ierr = (*draw->ops->resizewindow)(draw,w,h);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawGetWindowSize"
/*@
   PetscDrawGetWindowSize - Gets the size of the window.

   Not collective

   Input Parameter:
.  draw - the window

   Output Parameters:
.  w,h - the window width and height

   Level: intermediate

.seealso: PetscDrawResizeWindow(), PetscDrawCheckResizedWindow()
@*/
PetscErrorCode  PetscDrawGetWindowSize(PetscDraw draw,int *w,int *h)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  if (w) PetscValidPointer(w,2);
  if (h) PetscValidPointer(h,3);
  if (w) *w = draw->w;
  if (h) *h = draw->h;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawCheckResizedWindow"
/*@
   PetscDrawCheckResizedWindow - Checks if the user has resized the window.

   Collective on PetscDraw

   Input Parameter:
.  draw - the window

   Level: advanced

.seealso: PetscDrawResizeWindow()

@*/
PetscErrorCode  PetscDrawCheckResizedWindow(PetscDraw draw)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  if (draw->ops->checkresizedwindow) {
    ierr = (*draw->ops->checkresizedwindow)(draw);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawGetTitle"
/*@C
   PetscDrawGetTitle - Gets pointer to title of a PetscDraw context.

   Not collective

   Input Parameter:
.  draw - the graphics context

   Output Parameter:
.  title - the title

   Level: intermediate

.seealso: PetscDrawSetTitle()
@*/
PetscErrorCode  PetscDrawGetTitle(PetscDraw draw,char **title)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  PetscValidPointer(title,2);
  *title = draw->title;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawSetTitle"
/*@C
   PetscDrawSetTitle - Sets the title of a PetscDraw context.

   Collective on PetscDraw

   Input Parameters:
+  draw - the graphics context
-  title - the title

   Level: intermediate

   Note: The title is positioned in the windowing system title bar for the window. Hence it will not be saved with -draw_save
   in the image.

   A copy of the string is made, so you may destroy the
   title string after calling this routine.

   You can use PetscDrawAxisSetLabels() to indicate a title within the window

.seealso: PetscDrawGetTitle(), PetscDrawAppendTitle()
@*/
PetscErrorCode  PetscDrawSetTitle(PetscDraw draw,const char title[])
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  PetscValidCharPointer(title,2);
  ierr = PetscFree(draw->title);CHKERRQ(ierr);
  ierr = PetscStrallocpy(title,&draw->title);CHKERRQ(ierr);
  if (draw->ops->settitle) {
    ierr = (*draw->ops->settitle)(draw,draw->title);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawAppendTitle"
/*@C
   PetscDrawAppendTitle - Appends to the title of a PetscDraw context.

   Collective on PetscDraw

   Input Parameters:
+  draw - the graphics context
-  title - the title

   Note:
   A copy of the string is made, so you may destroy the
   title string after calling this routine.

   Level: advanced

.seealso: PetscDrawSetTitle(), PetscDrawGetTitle()
@*/
PetscErrorCode  PetscDrawAppendTitle(PetscDraw draw,const char title[])
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  if (title) PetscValidCharPointer(title,2);
  if (!title || !title[0]) PetscFunctionReturn(0);

  if (draw->title) {
    size_t len1,len2;
    char   *newtitle;
    ierr = PetscStrlen(title,&len1);CHKERRQ(ierr);
    ierr = PetscStrlen(draw->title,&len2);CHKERRQ(ierr);
    ierr = PetscMalloc1(len1 + len2 + 1,&newtitle);CHKERRQ(ierr);
    ierr = PetscStrcpy(newtitle,draw->title);CHKERRQ(ierr);
    ierr = PetscStrcat(newtitle,title);CHKERRQ(ierr);
    ierr = PetscFree(draw->title);CHKERRQ(ierr);
    draw->title = newtitle;
  } else {
    ierr = PetscStrallocpy(title,&draw->title);CHKERRQ(ierr);
  }
  if (draw->ops->settitle) {
    ierr = (*draw->ops->settitle)(draw,draw->title);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawDestroy_Private"
static PetscErrorCode PetscDrawDestroy_Private(PetscDraw draw)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (!draw->ops->save && !draw->ops->getimage) PetscFunctionReturn(0);
  ierr = PetscDrawSaveMovie(draw);CHKERRQ(ierr);
  if (draw->savefinalfilename) {
    draw->savesinglefile = PETSC_TRUE;
    ierr = PetscDrawSetSave(draw,draw->savefinalfilename);CHKERRQ(ierr);
    ierr = PetscDrawSave(draw);CHKERRQ(ierr);
  }
  ierr = PetscBarrier((PetscObject)draw);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawDestroy"
/*@
   PetscDrawDestroy - Deletes a draw context.

   Collective on PetscDraw

   Input Parameters:
.  draw - the drawing context

   Level: beginner

.seealso: PetscDrawCreate()

@*/
PetscErrorCode  PetscDrawDestroy(PetscDraw *draw)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (!*draw) PetscFunctionReturn(0);
  PetscValidHeaderSpecific(*draw,PETSC_DRAW_CLASSID,1);
  if (--((PetscObject)(*draw))->refct > 0) PetscFunctionReturn(0);

  if ((*draw)->pause == -2) {
    (*draw)->pause = -1;
    ierr = PetscDrawPause(*draw);CHKERRQ(ierr);
  }

  /* if memory was published then destroy it */
  ierr = PetscObjectSAWsViewOff((PetscObject)*draw);CHKERRQ(ierr);

  ierr = PetscDrawDestroy_Private(*draw);CHKERRQ(ierr);

  if ((*draw)->ops->destroy) {
    ierr = (*(*draw)->ops->destroy)(*draw);CHKERRQ(ierr);
  }
  ierr = PetscDrawDestroy(&(*draw)->popup);CHKERRQ(ierr);
  ierr = PetscFree((*draw)->title);CHKERRQ(ierr);
  ierr = PetscFree((*draw)->display);CHKERRQ(ierr);
  ierr = PetscFree((*draw)->savefilename);CHKERRQ(ierr);
  ierr = PetscFree((*draw)->saveimageext);CHKERRQ(ierr);
  ierr = PetscFree((*draw)->savemovieext);CHKERRQ(ierr);
  ierr = PetscFree((*draw)->savefinalfilename);CHKERRQ(ierr);
  ierr = PetscHeaderDestroy(draw);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawGetPopup"
/*@
   PetscDrawGetPopup - Creates a popup window associated with a PetscDraw window.

   Collective on PetscDraw

   Input Parameter:
.  draw - the original window

   Output Parameter:
.  popup - the new popup window

   Level: advanced

@*/
PetscErrorCode  PetscDrawGetPopup(PetscDraw draw,PetscDraw *popup)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  PetscValidPointer(popup,2);

  if (draw->popup) *popup = draw->popup;
  else if (draw->ops->getpopup) {
    ierr = (*draw->ops->getpopup)(draw,popup);CHKERRQ(ierr);
    if (*popup) {
      ierr = PetscObjectSetOptionsPrefix((PetscObject)*popup,"popup_");CHKERRQ(ierr);
      (*popup)->pause = 0.0;
      ierr = PetscDrawSetFromOptions(*popup);CHKERRQ(ierr);
    }
  } else *popup = NULL;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawSetDisplay"
/*@
  PetscDrawSetDisplay - Sets the display where a PetscDraw object will be displayed

  Input Parameter:
+ draw - the drawing context
- display - the X windows display

  Level: advanced

@*/
PetscErrorCode  PetscDrawSetDisplay(PetscDraw draw,const char display[])
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFree(draw->display);CHKERRQ(ierr);
  ierr = PetscStrallocpy(display,&draw->display);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "PetscDrawSetDoubleBuffer"
/*@
   PetscDrawSetDoubleBuffer - Sets a window to be double buffered.

   Logically Collective on PetscDraw

   Input Parameter:
.  draw - the drawing context

   Level: intermediate

   Concepts: drawing^double buffer
   Concepts: graphics^double buffer
   Concepts: double buffer

@*/
PetscErrorCode  PetscDrawSetDoubleBuffer(PetscDraw draw)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  if (draw->ops->setdoublebuffer) {
    ierr = (*draw->ops->setdoublebuffer)(draw);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawGetSingleton"
/*@C
   PetscDrawGetSingleton - Gain access to a PetscDraw object as if it were owned
        by the one process.

   Collective on PetscDraw

   Input Parameter:
.  draw - the original window

   Output Parameter:
.  sdraw - the singleton window

   Level: advanced

.seealso: PetscDrawRestoreSingleton(), PetscViewerGetSingleton(), PetscViewerRestoreSingleton()

@*/
PetscErrorCode  PetscDrawGetSingleton(PetscDraw draw,PetscDraw *sdraw)
{
  PetscErrorCode ierr;
  PetscMPIInt    size;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  PetscValidPointer(sdraw,2);

  ierr = MPI_Comm_size(PetscObjectComm((PetscObject)draw),&size);CHKERRQ(ierr);
  if (size == 1) {
    ierr = PetscObjectReference((PetscObject)draw);CHKERRQ(ierr);
    *sdraw = draw;
  } else {
    if (draw->ops->getsingleton) {
      ierr = (*draw->ops->getsingleton)(draw,sdraw);CHKERRQ(ierr);
    } else SETERRQ1(PETSC_COMM_SELF,PETSC_ERR_SUP,"Cannot get singleton for this type %s of draw object",((PetscObject)draw)->type_name);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscDrawRestoreSingleton"
/*@C
   PetscDrawRestoreSingleton - Remove access to a PetscDraw object as if it were owned
        by the one process.

   Collective on PetscDraw

   Input Parameters:
+  draw - the original window
-  sdraw - the singleton window

   Level: advanced

.seealso: PetscDrawGetSingleton(), PetscViewerGetSingleton(), PetscViewerRestoreSingleton()

@*/
PetscErrorCode  PetscDrawRestoreSingleton(PetscDraw draw,PetscDraw *sdraw)
{
  PetscErrorCode ierr;
  PetscMPIInt    size;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(draw,PETSC_DRAW_CLASSID,1);
  PetscValidPointer(sdraw,2);
  PetscValidHeaderSpecific(*sdraw,PETSC_DRAW_CLASSID,2);

  ierr = MPI_Comm_size(PetscObjectComm((PetscObject)draw),&size);CHKERRQ(ierr);
  if (size == 1) {
    if (draw == *sdraw) {
      ierr = PetscObjectDereference((PetscObject)draw);CHKERRQ(ierr);
      *sdraw = NULL;
    } else SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_WRONG,"Cannot restore singleton, it is not the parent draw");
  } else {
    if (draw->ops->restoresingleton) {
      ierr = (*draw->ops->restoresingleton)(draw,sdraw);CHKERRQ(ierr);
    } else SETERRQ1(PETSC_COMM_SELF,PETSC_ERR_SUP,"Cannot restore singleton for this type %s of draw object",((PetscObject)draw)->type_name);
  }
  PetscFunctionReturn(0);
}
