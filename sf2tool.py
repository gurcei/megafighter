#!/usr/local/bin/python
import wx
import jsonpickle
import os.path
import glob
import enum
import sys
from wx.lib.anchors import LayoutAnchors
from numpy import array


# -----------------------------------------
# MY SETTINGS-RELATED CLASSES
# -----------------------------------------

class Settings:
  def __init__(self, projpath=''):
    if projpath=='':
      return

    self.projpath = projpath
    self.groups = {}

    # add in groups
    for dir in os.listdir(self.projpath):
      if not os.path.isdir(os.path.join(self.projpath,dir)):
        continue
      if dir[0] == '.':
        continue
      self.groups[dir] = Group(self.projpath, dir)

  def update(self):
    for dir in os.listdir(self.projpath):
      if not os.path.isdir(os.path.join(self.projpath, dir)):
        continue
      if dir[0] == '.':
        continue
      if dir in self.groups:
        self.groups[dir].update(self.projpath)
      else:
        self.groups[dir] = Group(self.projpath, dir)

# -----------------------------------------

class Group:
  def __init__(self, projpath, name):
    self.name = name
    groupPath = os.path.join(projpath, self.name)
    self.PNGs = self._find_pngs(groupPath)

  def update(self, projpath):
    groupPath = os.path.join(projpath, self.name)
    self._find_pngs(groupPath, refresh=True)

# - - - - - - - - - - - - - - - - - - -

  def _find_pngs(self, path, refresh=False):
    pngs = {}
    files = glob.glob(path + "/*.png")
    short_files = [f[len(path)+1:-4] for f in files]

    for f in short_files:
      # are we just refreshing existing list?
      if refresh:
        if not f in self.PNGs:
          self.PNGs[f] = PNG(f)
      # nope, we're creating a completely new list
      else:
        pngs[f] = PNG(f)

    return pngs

# -----------------------------------------

class PNG:
  def __init__(self, name):
    self.name=name
    self.hitboxes= { 'Head': [0,0,0,0], 'Torso': [0,0,0,0], 'Feet': [0,0,0,0], 'Attack': [0,0,0,0] }
    self.crop = [ ]
    self.adjust = [ ]

# -----------------------------------------

class Anim:
  name=""
  relx=[]
  rely=[]
  frame=[]
  animlength=0
  pngs=[]
  pingpong=0
  cols=0
  rows=0


# -----------------------------------------
# MYFRAME GUI CLASS
# -----------------------------------------

class MyFrame(wx.Frame):

  def __init__(self):
    # Had to pass in these weird arguments for super() in python2.7 (not needed in python3?)
    # https://stackoverflow.com/questions/38963018/typeerror-super-takes-at-least-1-argument-0-given-error-is-specific-to-any
    super(MyFrame, self).__init__(parent=None, title='SF2 Animation Tool', pos=(100,50), size=(1100,700))
    self.Bind(wx.EVT_CLOSE, self.OnFrameClose)

    self.mode = None

    panel = wx.Panel(self, size=(self.GetSize()[0]-10, self.GetSize()[1]-10))
    panel.SetAutoLayout(True)
    
    _, self.lstGroups = self.create_labeled_list_box(panel, label='GROUPS', pos=(5,0), size=(100,360), choices=[])
    self.lstGroups.Bind(wx.EVT_LISTBOX, self.OnLstGroupSelectionChanged)
    self.lstGroups.Bind(wx.EVT_SET_FOCUS, self.OnLstGroupSelectionChanged)

    self.lblPngs, self.lstPngs = self.create_labeled_list_box(panel, label='PNGS', pos=(105,0), size=(150,360), choices=[])
    self.lstPngs.Bind(wx.EVT_LISTBOX, self.OnLstPngsSelectionChanged)

    _, self.lstAnims = self.create_labeled_list_box(panel, label='ANIMS', pos=(255,0), size=(100,360), choices=['aaa', 'bbb'])
    self.lstAnims.Bind(wx.EVT_LISTBOX, self.OnLstAnimsSelectionChanged)
    # I think these two should be panes that I turn on/off based on whether a PNGS or ANIMS item is selected
    self.pnlHitboxes = self.create_hitboxes_panel(panel, pos=(355,0), size=(215,300))
    self.pnlAnimDetails = self.create_animdetails_panel(panel, pos=(355,0), size=(200,300))

    self.pnlHitboxes.Hide()
    self.pnlAnimDetails.Hide()

    self.txtDebug = wx.TextCtrl(panel, pos=(5,385), size=(555, 300))
    self.txtDebug.SetConstraints(LayoutAnchors(self.txtDebug, left=True, top=True, right=False, bottom=True))

    self.sheetscale = 1
    self.scale = 4
    self.bmp = self.create_bitmap_area(panel, pos=(575,0), size=(510,680))

    self.create_menu()

    self.Show()

  # - - - - - - - - - - - - - - - - - - -

  class Mode(enum.Enum):
    Group = 0
    Png = 1

  # - - - - - - - - - - - - - - - - - - -

  class Hitbox(enum.Enum):
    Head = 0
    Torso = 1
    Feet = 2
    Attack = 3

  # - - - - - - - - - - - - - - - - - - -

  def create_hitboxes_panel(self, panel, pos, size):
    hbpanel = wx.Panel(panel, pos=pos, size=size)
    itempos=[5, 20]
    self.lblPngName = self.create_static_field(hbpanel, tuple(itempos), "Name:", "???")

    itempos[1] += 30
    self.txtCrop = self.create_text_field(hbpanel, tuple(itempos), "Crop Dim:", "", self.OnBtnCutClicked)

    self.btnCut = wx.Button(hbpanel, label='Cut', pos=(itempos[0]+170,itempos[1]), size=(40,25))
    self.btnCut.Bind(event=wx.EVT_BUTTON, handler=self.OnBtnCutClicked)

    itempos[1] += 30
    self.lblPngSize = self.create_static_field(hbpanel, tuple(itempos), "Size:", "???")

    itempos[1] += 30
    self.txtAdjust = self.create_text_field(hbpanel, tuple(itempos), "Adjust:", "", self.OnBtnCutClicked)

    initialvalue="0, 0, 0, 0"
    self.lstTxtHboxes = []

    for hb in self.Hitbox:
      itempos[1] += 30
      self.lstTxtHboxes.append(self.create_text_field(hbpanel, tuple(itempos), hb.name + ":",
        initialvalue, self.OnTxtHboxEnter))

    return hbpanel

  # - - - - - - - - - - - - - - - - - - -

  def OnBtnCutClicked(self, event):
    print('clicked')
    # cut the image from the sprite-sheet at the coords specified by crop-dim field
    cropvals = [int(i) for i in self.txtCrop.GetValue().split(',')]
    adjustvals = None
    if len(self.txtAdjust.GetValue()) > 0:
      adjustvals = [int(i) for i in self.txtAdjust.GetValue().split(',')]
    [self.sx, self.sy, self.sw, self.sh] = cropvals
    self.update_cropdim(adjustvals)

    self.SaveOutCrop(self.sx, self.sy, self.sw, self.sh, self.pngPath, adjustvals)

    # redraw png
    self.OnLstPngsSelectionChanged(None)

  # - - - - - - - - - - - - - - - - - - -

  def SaveOutCrop(self, x, y, w, h, path, adjust=None):
    if not adjust:
      adjust = [ 0, 0, ((w+7)/8)*8, ((h+7)/8)*8 ]

    dbmp = wx.Bitmap(adjust[2], adjust[3], depth=16)

    sheetbmp = self.sprsheet.ConvertToBitmap()
    sourcedc = wx.MemoryDC(sheetbmp)
    pixels = self.sprsheet.GetData()
    transparent = (pixels[0], pixels[1], pixels[2])
    destdc=wx.MemoryDC(dbmp)
    destdc.SetBrush(wx.Brush(transparent))
    destdc.SetPen(wx.Pen(transparent, 1))
    destdc.DrawRectangle(0, 0, adjust[2], adjust[3])
    destdc.Blit(adjust[0], adjust[1], w, h, sourcedc, x, y, wx.COPY)

    # save out the new bitmap
    dbmp.SaveFile(path, wx.BITMAP_TYPE_PNG)

  # - - - - - - - - - - - - - - - - - - -

  def OnTxtHboxEnter(self, event):
    print(event.GetEventObject())
    self.update_image()
    self.CopyHitboxesFromGuiToSettings()
    projectNotSaved = True

  # - - - - - - - - - - - - - - - - - - -

  def create_animdetails_panel(self, panel, pos, size):
    adpanel = wx.Panel(panel, pos=pos, size=size)
    itempos=[5,20]
    self.txtRelx = self.create_text_field(adpanel, tuple(itempos), "relx[]:", "0, 0, 0, 0")
    itempos[1] += 30
    self.txtRely = self.create_text_field(adpanel, tuple(itempos), "rely[]:", "0, 0, 0, 0")
    itempos[1] += 30
    self.txtFrame = self.create_text_field(adpanel, tuple(itempos), "frame[]:", "0, 0, 0, 0")
    itempos[1] += 30
    self.lblAnimlen = self.create_static_field(adpanel, tuple(itempos), "anim_len:", "<implied>")
    itempos[1] += 30
    self.lblFrames = self.create_static_field(adpanel, tuple(itempos), "frames[]:", "<implied>")
    itempos[1] += 30
    self.txtPingPong = self.create_text_field(adpanel, tuple(itempos), "pingpong:", "0")
    itempos[1] += 30
    self.lblCols = self.create_static_field(adpanel, tuple(itempos), "cols:", "<implied>")
    itempos[1] += 30
    self.lblCols = self.create_static_field(adpanel, tuple(itempos), "rows:", "<implied>")
    return adpanel

  # - - - - - - - - - - - - - - - - - - -

  def create_static_field(self, panel, pos, name, value):
    relpos = list(pos)
    wx.StaticText(panel, pos=tuple(relpos), label=name)
    relpos[0] += 70
    field = wx.StaticText(panel, pos=tuple(relpos), label=value)
    return field

  # - - - - - - - - - - - - - - - - - - -

  def create_text_field(self, panel, pos, name, value, on_enter=None):
    relpos = list(pos)
    wx.StaticText(panel, pos=tuple(relpos), label=name)
    relpos[0] += 70

    style = wx.TE_PROCESS_ENTER if on_enter != None else wx.TE_LEFT
    field = wx.TextCtrl(panel, style=style, pos=tuple(relpos), size=(100,25), value=value)
    if on_enter != None:
      field.Bind(event=wx.EVT_TEXT_ENTER, handler=on_enter, source=field)
    return field

  # - - - - - - - - - - - - - - - - - - -

  def OnLstGroupSelectionChanged(self, event):
    self.movingPngFlag = False

    if type(event) == wx.FocusEvent:
      sel = self.lstGroups.GetSelection()
      if sel == -1:
        return
      self.selectedGroup = self.lstGroups.GetString(sel)
    else:
      self.selectedGroup = self.lstGroups.GetString(event.GetSelection())
    self.selectedGroupObj = settings.groups[self.selectedGroup]
    # show pngs within this sub-folder
    groupPath = os.path.join(settings.projpath, self.selectedGroup)

    if type(event) != wx.FocusEvent:
      self.ShowPngList()

    self.mode = self.Mode.Group
    # if there is a .gif within the Graphics/ folder with this group-name,
    # then let's assume it is the sprite-sheet and we can preview it
    if self.lstPngs.GetSelection() != -1 and hasattr(self.selectedPngObj, 'crop') and len(self.selectedPngObj.crop) > 0:
      c = self.selectedPngObj.crop
      self.sx = c[0]
      self.sy = c[1]
    else:
      self.sx = 0
      self.sy = 0
    self.DrawSpriteSheet()

  # - - - - - - - - - - - - - - - - - - -

  def ShowPngList(self):
    files = self.selectedGroupObj.PNGs
    self.lstPngs.Clear()
    self.lblPngs.SetLabel('PNGs')
    if len(files) != 0:
      items=files.keys()
      items.sort()
      self.lstPngs.InsertItems(items, 0)
      self.lblPngs.SetLabel('PNGs ({})'.format(len(items)))

  # - - - - - - - - - - - - - - - - - - -

  def DrawSpriteSheet(self):
    sprsheet = "{}.gif".format(self.selectedGroup)
    sprsheet = os.path.join(settings.projpath, sprsheet)
    if os.path.exists(sprsheet):
      self.load_plain_image(sprsheet)
      self.sprsheet = self.img
      self.OverlayPng()
    else:
      self.sprsheet = None

  # - - - - - - - - - - - - - - - - - - -

  def OverlayPng(self):
    bmp = self.img.ConvertToBitmap()
    dc=wx.MemoryDC(bmp)
    dc.SetPen(wx.Pen(wx.RED, 1, wx.PENSTYLE_SHORT_DASH))
    dc.SetBrush(wx.Brush(wx.RED, wx.BRUSHSTYLE_TRANSPARENT))
    sel = self.lstPngs.GetSelection()

    if sel != -1:
      pngPath = os.path.join(settings.projpath, self.selectedGroup, self.selectedPng + ".png")
      img = wx.Image(pngPath, wx.BITMAP_TYPE_ANY)
      sbmp = img.ConvertToBitmap()
      dt = img.GetData()
      sbmp.SetMaskColour((dt[0], dt[1], dt[2]))
      sourcedc = wx.MemoryDC(sbmp)
      self.sw = img.GetWidth()
      self.sh = img.GetHeight()
      dc.Blit(self.sx, self.sy, self.sw, self.sh, sourcedc, 0, 0, wx.COPY, useMask=True)
      dc.DrawRectangle(self.sx, self.sy, self.sw, self.sh)

    if hasattr(self, 'selNewPngFlag') and self.selNewPngFlag:

      dc.DrawRectangle(self.pt1[0] / self.sheetscale, self.pt1[1] / self.sheetscale,
          (self.pt2[0] - self.pt1[0]) / self.sheetscale,
          (self.pt2[1] - self.pt1[1]) / self.sheetscale)

    img = bmp.ConvertToImage()
    width = img.GetWidth() * self.sheetscale
    height = img.GetHeight() * self.sheetscale
    simg = img.Scale(width, height, wx.IMAGE_QUALITY_NORMAL)

    self.bmp.SetBitmap(simg.ConvertToBitmap())
    self.update_scrollbars()

  # - - - - - - - - - - - - - - - - - - -
  
  def _UpdatePngDetails(self):
    # import pdb; pdb.set_trace()
    self.selectedPng = self.lstPngs.GetString(self.lstPngs.GetSelection())
    self.lblPngName.SetLabel(self.selectedPng)
    self.selectedPngObj = self.selectedGroupObj.PNGs[self.selectedPng]
    self.CopyHitboxesFromSettingsToGui()

    if hasattr(self.selectedPngObj, 'crop') and len(self.selectedPngObj.crop) > 0:
      c = self.selectedPngObj.crop
      self.sx = c[0]
      self.sy = c[1]
      self.sw = c[2]
      self.sh = c[3]

      if len(c) > 0:
        self.txtCrop.SetValue('{}, {}, {}, {}'.format(c[0], c[1], c[2], c[3]))
      else:
        self.txtCrop.SetValue('')
    else:
      self.txtCrop.SetValue('')

  # - - - - - - - - - - - - - - - - - - -

  def OnLstPngsSelectionChanged(self, event):
    self.mode = self.Mode.Png
    self._UpdatePngDetails()
    self.pngPath = os.path.join(settings.projpath, self.selectedGroup, self.selectedPng + ".png")
    self.pnlHitboxes.Show()
    self.pnlAnimDetails.Hide()
    self.lstAnims.SetSelection(wx.NOT_FOUND)
    self.load_image(self.pngPath)
    self.lblPngSize.SetLabel("{}x{}".format(self.img.GetWidth(), self.img.GetHeight()))
    if hasattr(self.selectedPngObj, 'adjust'):
      a = self.selectedPngObj.adjust
      self.txtAdjust.SetValue("{}, {}, {}, {}".format(a[0], a[1], a[2], a[3]))

    # debug info
    dbg="ROWSEGS({}) = \n".format(len(self.rowsegs))
    for rowseg in self.rowsegs:
      dbg += str(rowseg) + "\n"

    dbg += "\n\nREPAIRS({}) = \n".format(len(self.repairs))
    for repair in self.repairs:
      dbg += str(repair) + "\n"

    self.txtDebug.SetValue(dbg)

  # - - - - - - - - - - - - - - - - - - -

  def CopyHitboxesFromSettingsToGui(self):
    hbidx = 0
    hbnames = [e.name for e in self.Hitbox]
    for txtHb in self.lstTxtHboxes:
      svals = [str(i) for i in self.selectedPngObj.hitboxes[hbnames[hbidx]]]
      ss = ', '.join(svals)
      txtHb.SetValue(ss)
      hbidx += 1

  # - - - - - - - - - - - - - - - - - - -

  def CopyHitboxesFromGuiToSettings(self):
    hbidx = 0
    hbnames = [e.name for e in self.Hitbox]
    for txtHb in self.lstTxtHboxes:
      vals = [int(i) for i in  txtHb.GetValue().split(',')]
      self.selectedPngObj.hitboxes[hbnames[hbidx]] = vals
      hbidx += 1

  # - - - - - - - - - - - - - - - - - - -

  def OnLstAnimsSelectionChanged(self, event):
    self.selectedAnim = self.lstAnims.GetString(event.GetSelection())
    self.pnlHitboxes.Hide()
    self.lstPngs.SetSelection(wx.NOT_FOUND)
    self.pnlAnimDetails.Show()

  # - - - - - - - - - - - - - - - - - - -

  def AddGroup(self, name):
    self.lstGroups.InsertItems([str(name)],self.lstGroups.GetCount())

  # - - - - - - - - - - - - - - - - - - -

  def OnFrameClose(self, event):
    global projectNotSaved
    if (type(event) == wx.CommandEvent or (type(event) == wx.CloseEvent and event.CanVeto())) and projectNotSaved:
      if wx.MessageBox("The project has not been saved... Continue to save and close?",
          "Please confirm",
          wx.ICON_QUESTION | wx.YES_NO) != wx.YES:
        if type(event) == wx.CloseEvent:
          event.Veto()
          return
        SaveSettings()
        return

    SaveSettings()
    self.Destroy()

  # - - - - - - - - - - - - - - - - - - -

  def create_labeled_list_box(self, panel, label, pos, size, choices):
    lbl = wx.StaticText(panel, pos=pos, label=label)
    pos = (pos[0], pos[1] + 18)
    return lbl, wx.ListBox(panel, pos=pos, size=size, choices=choices, style=0)

  # - - - - - - - - - - - - - - - - - - -

  def create_menu(self):
    menu_bar = wx.MenuBar()
    file_menu = wx.Menu()
    options_menu = wx.Menu()
    mnuitmFileOpenFolder = file_menu.Append(wx.ID_ANY, 'Open Folder\tCTRL-O', 'Open a folder with MP3s')
    mnuitmFileScanForNewItems = file_menu.Append(wx.ID_ANY, 'Scan for New Items\tF5')
    mnuitmFileSaveHitboxes = file_menu.Append(wx.ID_ANY, 'Save Hitboxes\tCTRL-S', "Save hitboxes to 'hitboxes.h'")
    self.toggle_view = options_menu.AppendCheckItem(wx.ID_ANY, 'Toggle View\tCTRL-t', 'Toggle between png and c64 view')
    self.toggle_colours = options_menu.AppendCheckItem(wx.ID_ANY, 'Toggle Colours\tCTRL-R', 'Toggle colours on/off')
    self.toggle_grid = options_menu.AppendCheckItem(wx.ID_ANY, 'Toggle Grid\tCTRL-G', 'Toggle grid')
    self.zoom_in = options_menu.Append(wx.ID_ANY, 'Zoom in\tCtrl-.', 'Zoom in')
    self.zoom_out = options_menu.Append(wx.ID_ANY, 'Zoom out\tCtrl-,', 'Zoom out')

    menu_bar.Append(file_menu, 'File')
    self.Bind(event=wx.EVT_MENU, handler=self.OnOpenFolder, source=mnuitmFileOpenFolder)
    self.Bind(event=wx.EVT_MENU, handler=self.OnScanForNewItems, source=mnuitmFileScanForNewItems)
    self.Bind(event=wx.EVT_MENU, handler=self.OnSaveHitboxes, source=mnuitmFileSaveHitboxes)

    menu_bar.Append(options_menu, 'Options')
    self.Bind(event=wx.EVT_MENU, handler=self.OnToggleView, source=self.toggle_view)
    self.Bind(event=wx.EVT_MENU, handler=self.OnToggleColour, source=self.toggle_colours)
    self.Bind(event=wx.EVT_MENU, handler=self.OnToggleGrid, source=self.toggle_grid)
    self.Bind(event=wx.EVT_MENU, handler=self.OnZoomIn, source=self.zoom_in)
    self.Bind(event=wx.EVT_MENU, handler=self.OnZoomOut, source=self.zoom_out)

	# Check for unique mac-osx 'apple menu'
    apple_menu = menu_bar.OSXGetAppleMenu()
    if apple_menu:
        quit_menu_item = apple_menu.FindItemByPosition(apple_menu.GetMenuItemCount()-1)
        self.Bind(wx.EVT_MENU, self.OnFrameClose, quit_menu_item)

    self.SetMenuBar(menu_bar)

  # - - - - - - - - - - - - - - - - - - -

  def OnZoomIn(self, event):
    if self.mode == self.Mode.Png:
      self.scale += 1
      self.load_image()
    elif self.mode == self.Mode.Group:
      self.sheetscale += 1
      self.DrawSpriteSheet()

  # - - - - - - - - - - - - - - - - - - -

  def OnZoomOut(self, event):
    if self.mode == self.Mode.Png:
      if self.scale > 1:
        self.scale -= 1
        self.load_image()
    elif self.mode == self.Mode.Group:
      if self.sheetscale > 1:
        self.sheetscale -= 1
        self.DrawSpriteSheet()

  # - - - - - - - - - - - - - - - - - - -

  def OnToggleGrid(self, event):
    self.ConvertToC64()
    self.update_image()

  # - - - - - - - - - - - - - - - - - - -

  def OnToggleView(self, event):
    self.update_image()
 
  def OnToggleColour(self, event):
    self.ConvertToC64()
    self.update_image()

  # - - - - - - - - - - - - - - - - - - -

  def OnOpenFolder(self, event):
    title = "Choose a directory:"
    dlg = wx.DirDialog(self, title, style=wx.DD_DEFAULT_STYLE)
    if dlg.ShowModal() == wx.ID_OK:
      SetGraphicsDirectory(dlg.GetPath())

  # - - - - - - - - - - - - - - - - - - -
  def OnScanForNewItems(self, event):
    global settings, projectNotSaved
    settings.update()
    projectNotSaved = True

    UpdateLists()

  # - - - - - - - - - - - - - - - - - - -

  def OnSaveHitboxes(self, event):
    global settings
    # read out all hitbox values for each group, and write them to a file
    outfile = open('hitboxes.h', 'wt')
    outfile.write("""typedef struct
{
  char name[64];
  unsigned short boxes[4][4];
} type_hitbox;

type_hitbox lstHitBoxes[] =
{\n""")
    for group in settings.groups:
      for png in settings.groups[group].PNGs:
        validflag = False
        hbs = settings.groups[group].PNGs[png].hitboxes
        for hb in hbs.values():
          if hb != [0, 0, 0, 0]:
            validflag = True
        if validflag:
          # write this one to file
          outfile.write("  {\n")
          outfile.write('    \"Graphics/{}/{}.png\",\n'.format(group, png))
          outfile.write('    ' + str(hbs['Head'])[1:-1] + ',\n')
          outfile.write('    ' + str(hbs['Torso'])[1:-1] + ',\n')
          outfile.write('    ' + str(hbs['Feet'])[1:-1] + ',\n')
          outfile.write('    ' + str(hbs['Attack'])[1:-1] + '\n')
          outfile.write('  },\n')

      print group

    outfile.write("""  { 0 }
    };""")

  # - - - - - - - - - - - - - - - - - - -

  def create_bitmap_area(self, panel, pos, size):
    self.spnl = wx.ScrolledWindow(panel, pos=pos, size=size)
    bmp = wx.StaticBitmap(parent=self.spnl)
    self.spnl.SetConstraints(LayoutAnchors(self.spnl, left=True, top=True, right=True, bottom=True))

    # mouse-related events
    bmp.Bind(wx.EVT_MOUSE_EVENTS, self.OnBmpMouseEvents)
    bmp.Bind(wx.EVT_MOTION, self.OnBmpMouseMove)
    self.spnl.Bind(wx.EVT_KEY_DOWN, self.OnSpnlKeyDown)
    self.spnl.Bind(wx.EVT_MOUSE_EVENTS, self.OnSpnlMouseEvents)
    return bmp

  # - - - - - - - - - - - - - - - - - - -

  def OnSpnlMouseEvents(self, event):
    if event.LeftUp() and self.movingPngFlag:
      self.movingPngFlag = False
      self.sx = int(self.sx)
      self.sy = int(self.sy)
      self.update_cropdim()
    event.Skip()

  # - - - - - - - - - - - - - - - - - - -

  def OnSpnlKeyDown(self, event):
    if event.ControlDown():
      c = event.GetKeyCode()
      # go to next png?
      if event.ShiftDown():
        idx = self.lstPngs.GetSelection()
        if c == wx.WXK_UP:
          if idx > 0:
            idx -= 1
            self.lstPngs.SetSelection(idx)
          else:
            return
        elif c == wx.WXK_DOWN:
          if idx < self.lstPngs.GetCount()-1:
            idx += 1
            self.lstPngs.SetSelection(idx)
          else:
            return
        else:
          return
        self._UpdatePngDetails()
      # move the overlay?
      elif c == wx.WXK_UP:
        self.sy -= 1
      elif c == wx.WXK_DOWN:
        self.sy += 1
      elif c == wx.WXK_LEFT:
        self.sx -= 1
      elif c == wx.WXK_RIGHT:
        self.sx += 1
      else:
        event.Skip()
        return
    else:
      return

    # import pdb; pdb.set_trace()
    self.DrawSpriteSheet()
    self.update_cropdim()

  # - - - - - - - - - - - - - - - - - - -

  def draw_hitboxes(self):
    realbmp = self.bmp.GetBitmap()
    dc=wx.MemoryDC(realbmp)
    dc.SetPen(wx.Pen(wx.RED, 1))
    dc.SetBrush(wx.Brush(wx.RED))
    for txtHb in self.lstTxtHboxes:
      coord = [int(i)*self.scale for i in txtHb.GetValue().split(',')]
      # draw thickened rectangle
      dc.DrawRectangle(coord[0], coord[1], coord[2]-coord[0]+self.scale, self.scale)
      dc.DrawRectangle(coord[0], coord[3], coord[2]-coord[0]+self.scale, self.scale)
      dc.DrawRectangle(coord[0], coord[1], self.scale, coord[3]-coord[1])
      dc.DrawRectangle(coord[2], coord[1], self.scale, coord[3]-coord[1])

    # dc.SelectObject(wx.NullBitmap)
    self.bmp.SetBitmap(realbmp)

  # - - - - - - - - - - - - - - - - - - -

  def update_scrollbars(self):
    posx = self.spnl.GetScrollPos(wx.HORIZONTAL)
    posy = self.spnl.GetScrollPos(wx.VERTICAL)
    self.spnl.Scroll(0,0)
    b = self.bmp.GetBitmap()
    self.spnl.SetScrollbars(10,10, b.GetWidth()/10+1, b.GetHeight()/10+1)
    self.spnl.SetScrollRate(10,10)
    self.spnl.Scroll(posx, posy)

  # - - - - - - - - - - - - - - - - - - -

  def update_image(self):
    if self.toggle_view.IsChecked():
      self.bmp.SetBitmap(self.png)
    else:
      self.bmp.SetBitmap(self.c64bmp)

    self.draw_hitboxes()
    self.bmp.Refresh()

    self.update_scrollbars()

    # scale up
    # unfortunately, looks like I need to scale while it is still an image
    # as wx.Bitmap doesn't have a scale method (well, not wxpython versions below 4.1)
    # So I need to scale the image first and then apply the hitboxes (not the other way around)


  # - - - - - - - - - - - - - - - - - - -

  def IsTransparentPixel(self, pxl):
    return (pxl[0] == self.transparent[0] and pxl[1] == self.transparent[1] and pxl[2] == self.transparent[2])

  def Assess8by8Chunk(self, x, y, width, height, multiplier, pixels):
    charbytes = [0] * 8
    maskbytes = [0] * 8
    transparent_count = 0

    #if x == 32 and y == 16:
    #  import pdb; pdb.set_trace()

    for yd in range(0, 8):
      byteval = 0
      rowptr = (y+yd)*width*multiplier

      for xd in range(0, 8):
        ptr = rowptr + (x+xd)*multiplier
        pxl = pixels[ptr:ptr+3]

        istransparent = self.IsTransparentPixel(pxl)
        # check if it's a transparent pixel
        # print('{} : ({},{}) : {}'.format(ptr, x, y, repr(pxl)))
        if (y+yd) >= height or istransparent:
          transparent_count += 1
          maskbytes[yd] |= 1 << (7 - xd)

        if (y+yd) < height:
          # check threshold for black
          r=(pxl[0] + pxl[1] + pxl[2]) / 3
          if r < 80 and (x+xd < width) and not istransparent:
            byteval |= 1 << (7 - xd)
          elif r < 160 and (x+xd < width) or istransparent:
            if ((xd%2) and (yd%2)) or (not (xd%2) and not (yd%2)):
              byteval |= 1 << (7-xd)
      charbytes[yd] = byteval

    return charbytes, maskbytes, transparent_count

  def AppendCurSeg(self, rowsegs, curseg):
    rowsegs.append(curseg)
    #import pdb; pdb.set_trace()
    curseg = {
        'reloffset': 0,
        'length': 0
      }
    return curseg

  def CheckIfStartOrEndOfSegment(self, found_start, rowsegs, curseg):
    if found_start:
      found_start = False
      # if we already started a segment, then a transparent char (or mask-repair char)
      # indicates the end of a segment. So move to next segment
      curseg = self.AppendCurSeg(rowsegs, curseg)
    else:
      # if we haven't started a segment yet, then increment the upcoming
      # segment's reloffset
      curseg['reloffset'] += 8
      #import pdb; pdb.set_trace()

    return found_start, curseg

  def AppendMaskRepairChar(self, repairs, currepair, charbytes, maskbytes):
    # spawn a 'mask' repair character, instead of being part of the segment
    # currepair->reloffset should be already set
    for l in range(0, 8):
      currepair['vals'].append(charbytes[l])
      currepair['vals'].append(maskbytes[l])

    repairs.append(currepair)

    currepair = {
        'reloffset': 0,
        'vals': [ ]
        }

    return currepair

  def _ExtractC64ImgStructs(self):
    found_start = False
    out2name = ''
    MAX_SEGS=1000
    multiplier = 3 # for RGB (if RGBA, needs to be 4)

    rowsegs = []
    repairs = []
    currepair = {
        'reloffset': 0,
        'vals': [ ]
        }
    seggedbmp = {
        'num_segments': 0,
        'start_segment_idx': 0,
        'num_repairs': 0,
        'reu_ptr': 0
        }
    segdata = [ ] # the raw binary data of all segments

    width = self.img.GetWidth()
    height = self.img.GetHeight()

    pixels = self.img.GetData()

    curseg = {
        'reloffset': 0,
        'length': 0
      }

    # for now, let's assume that the top-left pixel is the transparency colour :)
    self.transparent = (pixels[0], pixels[1], pixels[2])

    #import pdb; pdb.set_trace()
    for y in range(0, height, 8):
      for x in range(0, width, 8):

        charbytes, maskbytes, transparent_count = \
            self.Assess8by8Chunk(x, y, width, height, multiplier, pixels)

        # if all transparent, skip this char and move onto the next
        if transparent_count == 64:
          found_start, curseg = self.CheckIfStartOrEndOfSegment(found_start, rowsegs, curseg)

        # if more than 16 pixels are transparent, spawn a 'mask' repair character
        elif transparent_count > 16:
          found_start, curseg = self.CheckIfStartOrEndOfSegment(found_start, rowsegs, curseg)

          currepair = self.AppendMaskRepairChar(repairs, currepair, charbytes, maskbytes)

        # this is a non-transparent char that needs to be added as segmentdata
        else:
          if not found_start:
            found_start = True
          # this is a non-transparent character, so let's save this data to the file
          segdata.extend(charbytes)
          curseg['length'] += 1

        currepair['reloffset'] += 8

      # if we finished this row and we're mid-segment, let's move to next segment
      if found_start:
        found_start = False
        curseg = self.AppendCurSeg(rowsegs, curseg)
        curseg['reloffset'] -= 8


      next_row_increment = 40*8 - ((width+7) / 8)*8
      curseg['reloffset'] += next_row_increment
      currepair['reloffset'] += next_row_increment

    # The next bit deals with hitboxes, but I'll skip that for now
    # (as we're not doing png->bin+hitbox data, but just png->bin for preview purposes)

    # no need for me to write out files such as:
    # - ryu_idle2.bin (the segdata for this frame)
    # - ryu_idle2.bmp_data (the metadata describing the segments and repairs this frame consists of)
    # - ryu_idle2.segs_meta (the metadata describing each individual segment)

    # but, again, for preview purposes, it'd be nice to preserve the bare minimum so I can draw it
    # segdata = contains what 'ryu_idle2.bin' would have contained
    # repairs = would have been concatended into segdata (after the hitbox data), but we can leave it separate here
    # segmeta = rowsegs

    return segdata, repairs, rowsegs

  def ConvertToC64Img(self):
    self.segdata, self.repairs, self.rowsegs = self._ExtractC64ImgStructs()

    # create a new monochrome image based on the returned structures
    width = self.img.GetWidth()
    height = self.img.GetHeight()
    img = wx.Image(width, height)

    pixels = img.GetData()
    byteoffset = 0

    def setpxl(pixels, x, y, r, g, b):
      ofs = (width*y + x)*3
      pixels[ofs] = r
      pixels[ofs+1] = g
      pixels[ofs+2] = b

    def drawchar(pixels, byteoffset, idx, r, g, b, masks=None):
      for yd in range(0, 8):
        yloc = byteoffset / (40*8) * 8
        xloc = byteoffset % (40*8) / 8 * 8

        byteval = self.segdata[idx]
        idx += 1
        maskval = masks[yd] if masks != None else 0
        clroff = [0, 0, 0]
        if masks != None and self.toggle_colours.IsChecked():
          clroff = [50,0,0]

        for xd in range(0, 8):
          # ignore masked out pixels
          if maskval & 128 == 0:
            if byteval & 128:
              setpxl(pixels, xloc+xd, yloc+yd, clroff[0], clroff[1], clroff[2])
            else:
              setpxl(pixels, xloc+xd, yloc+yd, r, g, b)

          byteval <<= 1
          maskval <<= 1

        byteoffset += 1
      return byteoffset, idx

    # fill with hash pattern
    for y in range(0, height):
      for x in range(0, width):
        setpxl(pixels, x, y, 100, 100, 100)
        #if (y % 2):
        #  setpxl(pixels, x, y, 255, 255, 255)
        #  setpxl(pixels, x+1, y, 0, 0, 0)
        #else:
        #  setpxl(pixels, x, y, 0, 0, 0)
        #  setpxl(pixels, x+1, y, 255, 255, 255)

    # draw segments
    idx = 0
    clr=[100,200,200] if self.toggle_colours.IsChecked() else [255,255,255]
    for cursegmeta in self.rowsegs:
      byteoffset += cursegmeta['reloffset'] # this is a byte-offset

      # print('{} : {},{}'.format(idx, xloc, yloc))
      # import pdb; pdb.set_trace()

      # draw current segment
      for charidx in range(0, cursegmeta['length']):
        byteoffset, idx = drawchar(pixels, byteoffset, idx, clr[0],clr[1],clr[2])

      byteoffset += 8

    byteoffset = 0
    clr=[255,100,100] if self.toggle_colours.IsChecked() else [255,255,255]
    for repair in self.repairs:
      byteoffset += repair['reloffset'] # this is a byte-offset
      self.segdata.append(repair['vals'][0])
      self.segdata.append(repair['vals'][2])
      self.segdata.append(repair['vals'][4])
      self.segdata.append(repair['vals'][6])
      self.segdata.append(repair['vals'][8])
      self.segdata.append(repair['vals'][10])
      self.segdata.append(repair['vals'][12])
      self.segdata.append(repair['vals'][14])
      masks = repair['vals'][1::2]
      _, idx = drawchar(pixels, byteoffset, idx, clr[0], clr[1], clr[2], masks)

    img.SetData(pixels)
    return img


  # - - - - - - - - - - - - - - - - - - -

  def _DrawGrid(self, bmp):
    dc=wx.MemoryDC(bmp)
    dc.SetPen(wx.Pen(wx.Colour(180,180,180), 1))
    dc.SetBrush(wx.Brush(wx.RED))

    width = self.img.GetWidth() * self.scale
    height = self.img.GetHeight() * self.scale

    for y in range(0,height,8*self.scale):
      dc.DrawLine(0, y, width, y)

    for x in range(0,width,8*self.scale):
      dc.DrawLine(x, 0, x, height)

  # - - - - - - - - - - - - - - - - - - -

  def ConvertToC64(self):
    width = self.img.GetWidth() * self.scale
    height = self.img.GetHeight() * self.scale
    self.c64img = self.ConvertToC64Img()
    self.c64bmp = self.c64img.Scale(width, height, wx.IMAGE_QUALITY_NORMAL).ConvertToBitmap()

    if self.toggle_grid.IsChecked():
      self._DrawGrid(self.c64bmp)

  def load_plain_image(self, imgpath):
    img = wx.Image(imgpath, wx.BITMAP_TYPE_ANY)
    self.img = img

  def load_image(self, imgpath=None):
    if imgpath != None:
      img = wx.Image(imgpath, wx.BITMAP_TYPE_ANY)
      self.img = img
    width = self.img.GetWidth() * self.scale
    height = self.img.GetHeight() * self.scale
    simg = self.img.Scale(width, height, wx.IMAGE_QUALITY_NORMAL)
    self.png = simg.ConvertToBitmap()

    try:
      self.ConvertToC64()
    except:
      print("EXCEPTION in ConvertToC64()\n", sys.exc_info()[0])

    self.update_image()

  # - - - - - - - - - - - - - - - - - - -

  pt1 = (0,0)
  pt2 = (0,0)

  # - - - - - - - - - - - - - - - - - - -
  def update_hb_and_image(self):
    global projectNotSaved
    self.update_hb_coords()
    self.update_image()
    self.CopyHitboxesFromGuiToSettings()
    projectNotSaved = True

  # - - - - - - - - - - - - - - - - - - -

  def OnBmpMouseMove(self, event):
    if self.mode == self.Mode.Group:
      self.HandleMouseMoveInGroupMode(event)
    elif self.mode == self.Mode.Png:
      self.HandleMouseMoveInPngMode(event)

  # - - - - - - - - - - - - - - - - - - -

  def OnBmpMouseEvents(self, event):
    if self.mode == self.Mode.Group:
      self.HandleMouseEventsInGroupMode(event)
    elif self.mode == self.Mode.Png:
      self.HandleMouseEventsInPngMode(event)

    # I needed to do this, in order for the parent scroll-panel to be able to handle
    # horizontal+vertical scrolling gestures on my laptop's touchpad.
    event.Skip()

  # - - - - - - - - - - - - - - - - - - -

  def InPngBounds(self, pos):
    if hasattr(self, 'sw') and hasattr(self, 'sh'):
      if self.sx < pos[0]/self.sheetscale and (self.sx+self.sw) > pos[0]/self.sheetscale and \
        self.sy < pos[1]/self.sheetscale and (self.sy+self.sh) > pos[1]/self.sheetscale:
        return True
      else:
        return False
    else:
      return False

  # - - - - - - - - - - - - - - - - - - -

  def HandleMouseEventsInGroupMode(self, event):
    global projectNotSaved
    if event.LeftDown() and not self.movingPngFlag:
      self.spnl.SetFocus()
      self.pt1 = array(event.GetPosition())
      self.movingPngFlag = False
      self.selNewPngFlag = False
      if self.InPngBounds(self.pt1):
        self.movingPngFlag = True
      else:
        self.selNewPngFlag = True

    if event.LeftUp():
      if self.movingPngFlag:
        self.movingPngFlag = False
        self.sx = int(self.sx)
        self.sy = int(self.sy)
        self.update_cropdim()
      elif self.selNewPngFlag:
        self.selNewPngFlag = False
        # Ask user to provide a name for the new png frame
        dlg = wx.TextEntryDialog(self, 'Name of new PNG frame:', value='')
        dlg.ShowModal()
        rslt = dlg.GetValue()
        if len(rslt) > 0:
          # if given, then cut it and save it to a new png file within the current group
          print(rslt)
          groupPath = os.path.join(settings.projpath, self.selectedGroup)
          fullpath = os.path.join(groupPath, rslt) + '.png'
          print(fullpath)
          x = self.pt1[0] / self.sheetscale
          y = self.pt1[1] / self.sheetscale
          width = (self.pt2[0]-self.pt1[0]) / self.sheetscale
          height = (self.pt2[1]-self.pt1[1]) / self.sheetscale
          self.SaveOutCrop(x, y, width, height, fullpath)
          self.selectedGroupObj.update(settings.projpath)
          selPngObj = self.selectedGroupObj.PNGs[rslt]
          selPngObj.crop = [ int(x), int(y), int(width), int(height) ]
          selPngObj.adjust = [ 0, 0, int(((width+7)/8)*8), int(((height+7)/8)*8) ]
          projectNotSaved = True
          self.ShowPngList()

  # - - - - - - - - - - - - - - - - - - -

  def update_cropdim(self, adjustvals=None):
    global projectNotSaved
    s = "{}, {}, {}, {}".format(self.sx, self.sy, self.sw, self.sh)
    self.txtCrop.SetValue(s)
    self.selectedPngObj.crop = [self.sx, self.sy, self.sw, self.sh]

    if adjustvals:
      s = "{}, {}, {}, {}".format(adjustvals[0], adjustvals[1], adjustvals[2], adjustvals[3])
      self.txtAdjust.SetValue(s)
      self.selectedPngObj.adjust = adjustvals

    projectNotSaved = True

  # - - - - - - - - - - - - - - - - - - -

  def HandleMouseMoveInGroupMode(self, event):
    pos = event.GetPosition()
    if event.LeftIsDown():
      if self.movingPngFlag:
        self.pt2 = array(event.GetPosition())
        diff = self.pt2 - self.pt1
        self.sx += diff[0]/self.sheetscale
        self.sy += diff[1]/self.sheetscale

        self.DrawSpriteSheet()
        self.pt1 = self.pt2
      elif self.selNewPngFlag:
        # Draw a red box around the selection
        self.pt2 = array(event.GetPosition())
        self.OverlayPng()

  # - - - - - - - - - - - - - - - - - - -

  def HandleMouseEventsInPngMode(self, event):
    if event.LeftDown():
      self.pt1 = event.GetPosition()
    if event.LeftUp():
      self.pt2 = event.GetPosition()
      self.update_hb_and_image()

  # - - - - - - - - - - - - - - - - - - -

  def HandleMouseMoveInPngMode(self, event):
    pos = event.GetPosition()
    if event.LeftIsDown():
      self.pt2 = event.GetPosition()
      self.update_hb_and_image()
    
  # - - - - - - - - - - - - - - - - - - -

  def update_hb_coords(self):
    s = "{}, {}, {}, {}".format(self.pt1[0]/self.scale, self.pt1[1]/self.scale,
        self.pt2[0]/self.scale, self.pt2[1]/self.scale)
    for txtHbox in self.lstTxtHboxes:
      if txtHbox.HasFocus():
        txtHbox.SetValue(s)
        return

# -----------------------------------------
# GLOBAL VARIABLES
# -----------------------------------------

settings = None
projectNotSaved = False
frame = None


# -----------------------------------------
# MAIN APP HELPER METHODS
# -----------------------------------------

def SetGraphicsDirectory(path):
  global settings, projectNotSaved, frame
  settings = Settings(path)
  projectNotSaved = True

  UpdateLists()

  # - - - - - - - - - - - - - - - - - - -

def UpdateLists():
  # search for sub-folders within here (each one will be a 'group')
  frame.lstGroups.Clear()
  frame.lblPngs.SetLabel('PNGs')
  frame.lstPngs.Clear()
  keys = settings.groups.keys()
  keys.sort()
  for dir in keys:
    frame.AddGroup(dir)

  # - - - - - - - - - - - - - - - - - - -

def SaveSettings():
  global settings
  if settings != None:
    with open("settings.json", "w") as f:
      f.write(jsonpickle.encode(settings,indent=4))

  # - - - - - - - - - - - - - - - - - - -

def LoadSettings():
  global settings
  if os.path.isfile("settings.json"):
    print("found settings.json")
    with open("settings.json", "r") as f:
      settings = jsonpickle.decode(f.read())

    # update gui too
    UpdateLists()

# -----------------------------------------
# MAIN
# -----------------------------------------
if __name__ == '__main__':
  app = wx.App()
  frame = MyFrame()
  frame.Show()
  LoadSettings()
  app.MainLoop()
