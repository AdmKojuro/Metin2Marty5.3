import item
import ui
import shop
import chat
import localeInfo
import app
#---<UiTooltip Functiones>---#
#----------------------------#
#----------------------------#

def FuncCheckPrice(self,slotIndex,price):
	if app.ENABLE_BUY_ITEMS_WORLDARD:
		vnum_special = shop.GetVnumBuy(slotIndex)
		count_special = shop.GetCountBuy(slotIndex)
		
		##chat.AppendChat(chat.CHAT_TYPE_INFO, "vnum especial %d"%vnum_special)
		##chat.AppendChat(chat.CHAT_TYPE_INFO, "precio cheque %d"%price_cheque)

		if vnum_special != 0:
			AppendPriceItem(self,vnum_special,count_special)
		else:
			self.AppendPrice(price)
	else:
		self.AppendPrice(price)

def AppendPriceItem(self,vnum_special,count_special):
	self.AppendSpace(5)
		
	item.SelectItem(vnum_special)

	itemImage = ui.ImageBox()
	itemImage.SetParent(self)
	itemImage.LoadImage(item.GetIconImageFileName())
	itemImage.SetPosition((self.toolTipWidth/2)-10, self.toolTipHeight)
	itemImage.Show()
	
	itemName = ui.TextLine()
	itemName.SetParent(itemImage)
	itemName.SetHorizontalAlignCenter()
	itemName.SetPosition(17,itemImage.GetHeight()+6)
	itemName.SetText(item.GetItemName())
	itemName.Show()
	
	itemCount = ui.TextLine()
	itemCount.SetParent(itemImage)
	itemCount.SetHorizontalAlignCenter()
	itemCount.SetPosition(18,itemImage.GetHeight()+17)
	itemCount.SetText("Precio: x%d"%count_special)
	itemCount.Show()

	self.toolTipHeight += itemImage.GetHeight()+28
	self.childrenList.append(itemImage)
	self.childrenList.append(itemName)
	self.childrenList.append(itemCount)
	self.ResizeToolTip()

#----------------------------#
#----------------------------#
#---<UiTooltip Functiones>---#


#---<LocaleInfo Functiones>--#
#----------------------------#
#----------------------------#

def FuncCheckMsg(slotPos,itemName,itemCount,itemPrice):
	if app.ENABLE_BUY_ITEMS_WORLDARD:
		vnum_special = shop.GetVnumBuy(slotPos)
		count_special = shop.GetCountBuy(slotPos)
		if vnum_special == 0:
			return localeInfo.DO_YOU_BUY_ITEM(itemName, itemCount, localeInfo.NumberToMoneyString(itemPrice))
		else:
			return DO_YOU_BUY_ITEM_SPECIAL(itemName, itemCount, vnum_special, count_special)
	else:
		return localeInfo.DO_YOU_BUY_ITEM(itemName, itemCount, localeInfo.NumberToMoneyString(itemPrice))
		
def DO_YOU_BUY_ITEM_SPECIAL(buyItemName, buyItemCount, buyItemVnumSpecial, buyItemSpecialCount):		
	item.SelectItem(buyItemVnumSpecial)
	itemName = item.GetItemName()

	if buyItemCount > 1 :
		return "Desea comprar %s x%d por %s x%d"%(buyItemName,buyItemCount,itemName,buyItemSpecialCount)
	else:
		return "Desea comprar %s por %s x%d"%(buyItemName,itemName,buyItemSpecialCount)

#---<LocaleInfo Functiones>--#
#----------------------------#
#----------------------------#

#---<Game Functiones>--#
#----------------------------#
#----------------------------#

def ShopErrorDict(type):
	if app.ENABLE_BUY_ITEMS_WORLDARD:
		if type == "NOT_ENOUGH_COUNT":
			return localeInfo.SHOP_NOT_ENOUGH_COUNT

	return localeInfo.SHOP_ERROR_DICT[type]

#---<Game Functiones>--#
#----------------------------#
#----------------------------#