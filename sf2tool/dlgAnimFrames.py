# A popup dialog for the user to select which frames he wants to be part of the animation
import wx

class dlgAnimFrames(wx.Dialog):

  def __init__(self):
    super(dlgAnimFrames, self).__init__(parent=None, title='Select Anim Frames', size=(400,700))
    self.lstPngs = wx.ListBox(self, pos=(5,5), size=(380,600))
    self.btnOK = wx.Button(self, label='OK', pos=(5, 650), size=(190,15))
    self.btnCancel = wx.Button(self, label='Cancel', pos=(205, 650), size=(190,15))


def ShowDialog():
  print('hello ShowDialog')
  dlg = dlgAnimFrames()
  dlg.ShowModal()
