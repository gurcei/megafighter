#!/usr/local/bin/python
import wx
import pickle
import os.path
import glob
import enum

# -----------------------------------------
# MY SETTINGS-RELATED CLASSES
# -----------------------------------------

class Settings:
  def __init__(self, projpath=''):
    self.projpath = projpath
    self.groups = {}

    # add in groups
    root, dirs, _ = os.walk(projpath)
    for dir in root[1]:
      self.groups[dir] = Group(projpath, dir)

# -----------------------------------------

class Group:
  def __init__(self, projpath, name):
    self.name = name
    groupPath = os.path.join(projpath, name)
    self.PNGs = self._find_pngs(groupPath)

# - - - - - - - - - - - - - - - - - - -

  def _find_pngs(self, path):
    pngs = {}
    files = glob.glob(path + "/*.png")
    short_files = [f[len(path)+1:-4] for f in files]

    for f in short_files:
      pngs[f] = PNG(f)

    return pngs

# -----------------------------------------

class PNG:
  def __init__(self, name):
    self.name=name
    self.hitboxes= { 'Head': [0,0,0,0], 'Torso': [0,0,0,0], 'Feet': [0,0,0,0], 'Attack': [0,0,0,0] }

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

    panel = wx.Panel(self)

    self.lstGroups = self.create_labeled_list_box(panel, label='GROUPS', pos=(5,0), size=(100,400), choices=[])
    self.lstGroups.Bind(wx.EVT_LISTBOX, self.OnGroupSelectionChanged)
    self.lstPngs = self.create_labeled_list_box(panel, label='PNGS', pos=(105,0), size=(150,400), choices=[])
    self.lstPngs.Bind(wx.EVT_LISTBOX, self.OnLstPngsSelectionChanged)
    self.lstAnims = self.create_labeled_list_box(panel, label='ANIMS', pos=(255,0), size=(100,400), choices=['aaa', 'bbb'])
    self.lstAnims.Bind(wx.EVT_LISTBOX, self.OnLstAnimsSelectionChanged)
    # I think these two should be panes that I turn on/off based on whether a PNGS or ANIMS item is selected
    self.pnlHitboxes = self.create_hitboxes_panel(panel, pos=(355,0), size=(200,300))
    self.pnlAnimDetails = self.create_animdetails_panel(panel, pos=(355,0), size=(200,300))

    self.pnlHitboxes.Hide()
    self.pnlAnimDetails.Hide()

    self.txtDebug = wx.TextCtrl(panel, pos=(5,410), size=(555, 280))

    self.scale = 4
    self.bmp = self.create_bitmap_area(panel, pos=(575,0), size=(200,200))

    self.create_menu()

    self.Show()

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

    initialvalue="0, 0, 0, 0"
    self.lstTxtHboxes = []

    for hb in self.Hitbox:
      itempos[1] += 30
      self.lstTxtHboxes.append(self.create_text_field(hbpanel, tuple(itempos), hb.name + ":",
        initialvalue, self.OnTxtHboxEnter))

    return hbpanel

  # - - - - - - - - - - - - - - - - - - -

  def OnTxtHboxEnter(self, event):
    print(event.GetEventObject())
    self.update_image()

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

  def OnGroupSelectionChanged(self, event):
    self.selectedGroup = self.lstGroups.GetString(event.GetSelection())
    self.selectedGroupObj = settings.groups[self.selectedGroup]
    # show pngs within this sub-folder
    groupPath = os.path.join(settings.projpath, self.selectedGroup)
    files = self.selectedGroupObj.PNGs
    self.lstPngs.Clear()
    if len(files) != 0:
      items=files.keys()
      items.sort()
      self.lstPngs.InsertItems(items, 0)

  # - - - - - - - - - - - - - - - - - - -

  def OnLstPngsSelectionChanged(self, event):
    self.selectedPng = self.lstPngs.GetString(event.GetSelection())
    self.selectedPngObj = self.selectedGroupObj.PNGs[self.selectedPng]
    pngPath = os.path.join(settings.projpath, self.selectedGroup, self.selectedPng + ".png")
    self.pnlHitboxes.Show()
    self.pnlAnimDetails.Hide()
    self.lblPngName.SetLabel(self.selectedPng)
    self.CopyHitboxesFromSettingsToGui()
    self.lstAnims.SetSelection(wx.NOT_FOUND)
    self.load_image(pngPath)

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
    wx.StaticText(panel, pos=pos, label=label)
    pos = (pos[0], pos[1] + 18)
    return wx.ListBox(panel, pos=pos, size=size, choices=choices, style=0)

  # - - - - - - - - - - - - - - - - - - -

  def create_menu(self):
    menu_bar = wx.MenuBar()
    file_menu = wx.Menu()
    open_folder_menu_item = file_menu.Append(wx.ID_ANY, 'Open Folder\tCTRL-O', 'Open a folder with MP3s')
    menu_bar.Append(file_menu, 'File')
    self.Bind(event=wx.EVT_MENU, handler=self.OnFileMenuOpenFolder, source=open_folder_menu_item)

	# Check for unique mac-osx 'apple menu'
    apple_menu = menu_bar.OSXGetAppleMenu()
    if apple_menu:
        quit_menu_item = apple_menu.FindItemByPosition(apple_menu.GetMenuItemCount()-1)
        self.Bind(wx.EVT_MENU, self.OnFrameClose, quit_menu_item)

    self.SetMenuBar(menu_bar)

  # - - - - - - - - - - - - - - - - - - -

  def OnFileMenuOpenFolder(self, event):
    title = "Choose a directory:"
    dlg = wx.DirDialog(self, title, style=wx.DD_DEFAULT_STYLE)
    if dlg.ShowModal() == wx.ID_OK:
      SetGraphicsDirectory(dlg.GetPath())

  # - - - - - - - - - - - - - - - - - - -

  def create_bitmap_area(self, panel, pos, size):
    bmp = wx.StaticBitmap(parent=panel, pos=pos, size=size)

    # mouse-related events
    bmp.Bind(wx.EVT_MOUSE_EVENTS, self.OnBmpMouseEvents)
    bmp.Bind(wx.EVT_MOTION, self.OnBmpMouseMove)
    return bmp

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

  def update_image(self):
    self.bmp.SetBitmap(self.png)
    self.draw_hitboxes()

    self.bmp.SetBitmap(self.c64bmp)
    # scale up
    # unfortunately, looks like I need to scale while it is still an image
    # as wx.Bitmap doesn't have a scale method (well, not wxpython versions below 4.1)
    # So I need to scale the image first and then apply the hitboxes (not the other way around)


  # - - - - - - - - - - - - - - - - - - -

  def IsTransparentPixel(self, pxl):
    return (pxl[0]==112 and pxl[1]==136 and pxl[2]==136) or (pxl[0] == 128 and pxl[1]==128 and pxl[2]==128)

  def Assess8by8Chunk(self, x, y, width, height, multiplier, pixels):
    charbytes = [0] * 8
    maskbytes = [0] * 8
    transparent_count = 0

    for yd in range(0, 8):
      byteval = 0
      rowptr = (y+yd)*width*multiplier

      for xd in range(0, 8):
        ptr = rowptr + (x+xd)*multiplier
        pxl = pixels[ptr:ptr+3]

        # check if it's a transparent pixel
        # import pdb; pdb.set_trace()
        # print('{} : ({},{}) : {}'.format(ptr, x, y, repr(pxl)))
        if (y+yd) >= height or self.IsTransparentPixel(pxl):
          transparent_count += 1
          maskbytes[yd] |= 1 << (7 - xd)

        if (y+yd) < height:
          # check threshold for black
          r=(pxl[0] + pxl[1] + pxl[2]) / 3
          if r < 80 and (x+xd < width):
            byteval |= 1 << (7 - xd)
          elif r < 160 and (x+xd < width):
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

    def drawchar(pixels, byteoffset, idx, r, g, b):
      for yd in range(0, 8):
        yloc = byteoffset / (40*8) * 8
        xloc = byteoffset % (40*8) / 8 * 8

        byteval = self.segdata[idx]
        idx += 1
        for xd in range(0, 8):
          if byteval & 128:
            setpxl(pixels, xloc+xd, yloc+yd, 0, 0, 0)
          else:
            setpxl(pixels, xloc+xd, yloc+yd, r, g, b)

          byteval <<= 1

        byteoffset += 1
      return byteoffset, idx

    # fill with hash pattern
    for y in range(0, height):
      for x in range(0, width,2):
        if (y % 2):
          setpxl(pixels, x, y, 255, 255, 255)
          setpxl(pixels, x+1, y, 0, 0, 0)
        else:
          setpxl(pixels, x, y, 0, 0, 0)
          setpxl(pixels, x+1, y, 255, 255, 255)

    # draw segments
    idx = 0
    for cursegmeta in self.rowsegs:
      byteoffset += cursegmeta['reloffset'] # this is a byte-offset

      # print('{} : {},{}'.format(idx, xloc, yloc))
      # import pdb; pdb.set_trace()

      # draw current segment
      for charidx in range(0, cursegmeta['length']):
        byteoffset, idx = drawchar(pixels, byteoffset, idx, 100,200,200)

      byteoffset += 8

    byteoffset = 0
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
      _, idx = drawchar(pixels, byteoffset, idx, 255, 100, 100)

    img.SetData(pixels)
    return img


  # - - - - - - - - - - - - - - - - - - -

  def load_image(self, imgpath):
    img = wx.Image(imgpath, wx.BITMAP_TYPE_ANY)
    self.img = img
    width = img.GetWidth() * self.scale
    height = img.GetHeight() * self.scale
    simg = img.Scale(width, height, wx.IMAGE_QUALITY_NORMAL)
    self.png = simg.ConvertToBitmap()

    self.c64img = self.ConvertToC64Img()
    self.c64bmp = self.c64img.Scale(width, height, wx.IMAGE_QUALITY_NORMAL).ConvertToBitmap()

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
    pos = event.GetPosition()
    if event.LeftIsDown():
      self.pt2 = event.GetPosition()
      self.update_hb_and_image()

  # - - - - - - - - - - - - - - - - - - -

  def OnBmpMouseEvents(self, event):
    if event.LeftDown():
      self.pt1 = event.GetPosition()
    if event.LeftUp():
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

      # search for sub-folders within here (each one will be a 'group')
      frame.lstGroups.Clear()
      for dir in settings.groups:
        frame.AddGroup(dir)

  # - - - - - - - - - - - - - - - - - - -

def SaveSettings():
  global settings
  if settings != None:
    pickle_out = open("settings.pickle", "wb")
    pickle.dump(settings, pickle_out)
    pickle_out.close()

  # - - - - - - - - - - - - - - - - - - -

def LoadSettings():
  global settings
  if os.path.isfile("settings.pickle"):
    print("found settings.pickle")
    pickle_in = open("settings.pickle", "rb")
    settings = pickle.load(pickle_in)

    # update gui too
    for group in settings.groups:
      frame.AddGroup(group)

# -----------------------------------------
# MAIN
# -----------------------------------------
if __name__ == '__main__':
  app = wx.App()
  frame = MyFrame()
  LoadSettings()
  app.MainLoop()
