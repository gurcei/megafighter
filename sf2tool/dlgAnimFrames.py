# A popup dialog for the user to select which frames he wants to be part of the animation
import wx

class dlgAnimFrames(wx.Dialog):

  def __init__(self, Pngs):
    super(dlgAnimFrames, self).__init__(parent=None, title='Select Anim Frames', size=(400,700))
    self.lstPngs = wx.ListBox(self, style=wx.LB_MULTIPLE, pos=(5,5), size=(380,600))
    self.btnOK = wx.Button(self, id=wx.ID_OK, label='OK', pos=(5, 650), size=(190,15))
    self.btnCancel = wx.Button(self, id=wx.ID_CANCEL, label='Cancel', pos=(205, 650), size=(190,15))

    if len(Pngs) != 0:
      self.lstPngs.InsertItems(Pngs, 0)

selectedItems=None

def ShowDialog(Pngs):
  global selectedItems
  print('hello ShowDialog')
  Pngs.sort()
  dlg = dlgAnimFrames(Pngs)
  rslt = dlg.ShowModal()
  if rslt == wx.ID_OK:
    selectedItems = [dlg.lstPngs.GetString(i) for i in dlg.lstPngs.GetSelections()]

  return rslt

def GetSelection():
  return selectedItems
