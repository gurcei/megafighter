#!/usr/local/bin/python
import wx

class MyFrame(wx.Frame):
  def __init__(self):
    # Had to pass in these weird arguments for super() in python2.7 (not needed in python3?)
    # https://stackoverflow.com/questions/38963018/typeerror-super-takes-at-least-1-argument-0-given-error-is-specific-to-any
    super(MyFrame, self).__init__(parent=None, title='SF2 Animation Tool')

    panel = wx.Panel(self)
    self.text_ctrl = wx.TextCtrl(panel, pos=(5, 5))
    my_btn = wx.Button(panel, label='Press Me', pos=(5, 55))
    my_btn.Bind(wx.EVT_BUTTON, self.on_press)

    self.list_ctrl = wx.ListCtrl(panel, pos=(5, 105), size=(-1, 100), style=wx.LC_REPORT | wx.BORDER_SUNKEN)
    self.list_ctrl.InsertColumn(0, 'Artist', width=100);
    self.list_ctrl.InsertColumn(1, 'Album', width=50);
    self.list_ctrl.InsertColumn(2, 'Title', width=50);

    self.create_menu()

    png = wx.Image("/Users/tramvo/Documents/gurce.png", wx.BITMAP_TYPE_ANY).ConvertToBitmap()
    wx.StaticBitmap(panel, -1, png, (10, 5), (png.GetWidth(), png.GetHeight()))

    # mouse-related events
    panel.Bind(wx.EVT_MOTION, self.on_move)
    self.Show()

  def on_move(self, event):
    pos = event.GetPosition()
    print(pos)

  def create_menu(self):
    menu_bar = wx.MenuBar()
    file_menu = wx.Menu()
    open_folder_menu_item = file_menu.Append(wx.ID_ANY, 'Open Folder', 'Open a folder with MP3s')
    menu_bar.Append(file_menu, '&File')
    self.Bind(event=wx.EVT_MENU, handler=self.on_open_folder, source=open_folder_menu_item)
    self.SetMenuBar(menu_bar)

  def on_open_folder(self, event):
    title = "Choose a directory:"
    dlg = wx.DirDialog(self, title, style=wx.DD_DEFAULT_STYLE)
    if dlg.ShowModal() == wx.ID_OK:
      print(dlg.GetPath())

  def on_press(self, event):
    value = self.text_ctrl.GetValue()
    if not value:
      print("You didn't enter anything!")
    else:
      print('You typed: "{}"'.format(value))


if __name__ == '__main__':
  app = wx.App()
  frame = MyFrame()
  app.MainLoop()
