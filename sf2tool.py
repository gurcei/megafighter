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
    self.groups = []

# - - - - - - - - - - - - - - - - - - -

class Group:
  name = ""
  PNGs = []

# - - - - - - - - - - - - - - - - - - -

class PNG:
  name=""
  hitboxes= { 'head': {0,0,0,0}, 'torso': {0,0,0,0}, 'feet': {0,0,0,0}, 'attack': {0,0,0,0} }

# - - - - - - - - - - - - - - - - - - -

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
    super(MyFrame, self).__init__(parent=None, title='SF2 Animation Tool', size=(1100,800))
    self.Bind(wx.EVT_CLOSE, self.OnClose)

    panel = wx.Panel(self)

    self.lstGroups = self.create_labeled_list_box(panel, label='GROUPS', pos=(5,0), size=(100,400), choices=[])
    self.lstGroups.Bind(wx.EVT_LISTBOX, self.GroupSelectionChanged)
    self.lstPngs = self.create_labeled_list_box(panel, label='PNGS', pos=(105,0), size=(150,400), choices=[])
    self.lstPngs.Bind(wx.EVT_LISTBOX, self.PngSelectionChanged)
    self.lstAnims = self.create_labeled_list_box(panel, label='ANIMS', pos=(255,0), size=(100,400), choices=['aaa', 'bbb'])
    # I think these two should be panes that I turn on/off based on whether a PNGS or ANIMS item is selected
    self.pnlHitboxes = self.create_hitboxes_panel(panel, pos=(355,0), size=(180,400))
    # self.lstAnimDets = self.create_labeled_list_box(panel, label='ANIMDETAILS', pos=(405,0), size=(100,400), choices=['111', '222'])

    # self.create_text_ctrl(panel)

    # self.create_list_ctrl(panel)

    self.create_menu()

    self.scale = 4
    self.bmp = self.create_bitmap_area(panel, pos=(555,0), size=(200,200))
    # self.draw_image("/Users/tramvo/c64/Projects/sf2/Graphics/Ryu/ryu_idle2.png")
    # self.create_image(panel)

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
    itempos=list(pos)
    itempos[0] = 5
    itempos[1] = 20
    self.hbname = self.create_static_field(hbpanel, tuple(itempos), "Name:", "???")

    initialvalue="0,0,0,0"
    self.hboxes = []

    for hb in self.Hitbox:
      itempos[1] += 30
      self.hboxes.append(self.create_text_field(hbpanel, tuple(itempos), hb.name + ":", initialvalue))

    return hbpanel

  # - - - - - - - - - - - - - - - - - - -

  def create_static_field(self, panel, pos, name, value):
    relpos = list(pos)
    wx.StaticText(panel, pos=tuple(relpos), label=name)
    relpos[0] += 50
    field = wx.StaticText(panel, pos=tuple(relpos), label=value)
    return field

  # - - - - - - - - - - - - - - - - - - -

  def create_text_field(self, panel, pos, name, value):
    relpos = list(pos)
    wx.StaticText(panel, pos=tuple(relpos), label=name)
    relpos[0] += 50
    field = wx.TextCtrl(panel, pos=tuple(relpos), size=(100,25), value=value)
    return field

  # - - - - - - - - - - - - - - - - - - -

  def GroupSelectionChanged(self, event):
    self.selectedGroup = self.lstGroups.GetString(event.GetSelection())
    # show pngs within this sub-folder
    groupPath = os.path.join(settings.projpath, self.selectedGroup)
    files = glob.glob(groupPath + "/*.png")
    short_files = [f[len(groupPath)+1:-4] for f in files]
    self.lstPngs.Clear()
    if len(short_files) != 0:
      self.lstPngs.InsertItems(short_files, 0)

  # - - - - - - - - - - - - - - - - - - -

  def PngSelectionChanged(self, event):
    self.selectedPng = self.lstPngs.GetString(event.GetSelection()) + ".png"
    pngPath = os.path.join(settings.projpath, self.selectedGroup, self.selectedPng)
    self.draw_image(pngPath)

  # - - - - - - - - - - - - - - - - - - -

  def AddGroup(self, name):
    self.lstGroups.InsertItems([str(name)],self.lstGroups.GetCount())

  # - - - - - - - - - - - - - - - - - - -

  def OnClose(self, event):
    global projectNotSaved
    if (type(event) == wx.CommandEvent or (type(event) == wx.CloseEvent and event.CanVeto())) and projectNotSaved:
      if wx.MessageBox("The project has not been saved... Continue closing?",
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

  def create_text_ctrl(self, panel):
    self.text_ctrl = wx.TextCtrl(panel, pos=(5, 5))
    my_btn = wx.Button(panel, label='Press Me', pos=(5, 55))
    my_btn.Bind(wx.EVT_BUTTON, self.on_press)

  def on_press(self, event):
    value = self.text_ctrl.GetValue()
    if not value:
      print("You didn't enter anything!")
    else:
      print('You typed: "{}"'.format(value))

  # - - - - - - - - - - - - - - - - - - -

  def create_list_ctrl(self, panel):
    self.list_ctrl = wx.ListCtrl(panel, pos=(5, 105), size=(-1, 100), style=wx.LC_REPORT | wx.BORDER_SUNKEN)
    self.list_ctrl.InsertColumn(0, 'Artist', width=100);
    self.list_ctrl.InsertColumn(1, 'Album', width=50);
    self.list_ctrl.InsertColumn(2, 'Title', width=50);

    self.list_ctrl.InsertItem(0, 'fish')
    self.list_ctrl.SetItem(0, 1, 'dish')
    self.list_ctrl.SetItem(0, 2, 'wish')

  # - - - - - - - - - - - - - - - - - - -

  def create_menu(self):
    menu_bar = wx.MenuBar()
    file_menu = wx.Menu()
    open_folder_menu_item = file_menu.Append(wx.ID_ANY, 'Open Folder\tCTRL-O', 'Open a folder with MP3s')
    menu_bar.Append(file_menu, 'File')
    self.Bind(event=wx.EVT_MENU, handler=self.on_open_folder, source=open_folder_menu_item)

	# Check for unique mac-osx 'apple menu'
    apple_menu = menu_bar.OSXGetAppleMenu()
    if apple_menu:
        quit_menu_item = apple_menu.FindItemByPosition(apple_menu.GetMenuItemCount()-1)
        self.Bind(wx.EVT_MENU, self.OnClose, quit_menu_item)

    self.SetMenuBar(menu_bar)

  def on_open_folder(self, event):
    title = "Choose a directory:"
    dlg = wx.DirDialog(self, title, style=wx.DD_DEFAULT_STYLE)
    if dlg.ShowModal() == wx.ID_OK:
      SetGraphicsDirectory(dlg.GetPath())

  # - - - - - - - - - - - - - - - - - - -

  def create_bitmap_area(self, panel, pos, size):
    bmp = wx.StaticBitmap(parent=panel, pos=pos, size=size)

    # mouse-related events
    bmp.Bind(wx.EVT_MOTION, self.on_move)
    return bmp

  # - - - - - - - - - - - - - - - - - - -

  def draw_image(self, imgpath):
    self.img = wx.Image(imgpath, wx.BITMAP_TYPE_ANY)
    width = self.img.GetWidth() * self.scale
    height = self.img.GetHeight() * self.scale
    simg = self.img.Scale(width, height, wx.IMAGE_QUALITY_NORMAL)

    png = simg.ConvertToBitmap()
    self.bmp.SetBitmap(png)

  def on_move(self, event):
    pos = event.GetPosition()
    print(pos)


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
      root, dirs, _ = os.walk(path)
      frame.lstGroups.Clear()
      for dir in root[1]:
        settings.groups.append(dir)
        frame.AddGroup(dir)


def SaveSettings():
  global settings
  if settings != None:
    pickle_out = open("settings.pickle", "wb")
    pickle.dump(settings, pickle_out)
    pickle_out.close()

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
