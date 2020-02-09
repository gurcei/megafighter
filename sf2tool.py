#!/usr/local/bin/python
import wx

class MyFrame(wx.Frame):
  def __init__(self):
    # Had to pass in these weird arguments for super() in python2.7 (not needed in python3?)
    # https://stackoverflow.com/questions/38963018/typeerror-super-takes-at-least-1-argument-0-given-error-is-specific-to-any
    super(MyFrame, self).__init__(parent=None, title='SF2 Animation Tool')
    self.Show()

if __name__ == '__main__':
  app = wx.App()
  frame = MyFrame()
  app.MainLoop()
