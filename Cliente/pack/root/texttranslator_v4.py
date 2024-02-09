# -*- coding: utf-8 -*-
import ui
import grp
import textwrap
import systemSetting
import uiToolTip
import time
import constInfo
import localeInfo
import json,requests,random,re
import urllib
import urllib3
import logging
import app

import sys
stdin, stdout, stderr = sys.stdin, sys.stdout, sys.stderr # added
reload(sys)
sys.stdin, sys.stdout, sys.stderr = stdin, stdout, stderr # added
sys.setdefaultencoding('utf8')
## Ente ente ente ente
    
# U CAN CHANGE THIS
DEFAULT_LANG_TO_ENCODING = "cp1252"

COLOR_INFO = grp.GenerateColor(1.0, 0.7843, 0.0, 1.0)
COLOR_TEXT = grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0)

TEXT_BUTTON_INFO = "Info"

SHOW_INFO_BUTTON_ON_BOARD = False
TOOLTIP_INFO_SHOW_TIME = 10.0
TOOLTIP_MULTI_TEXT_SHOW_TIME = 5.0
TOOLTIP_SHOW_TIME = 0.1

KEY_TO_TRANSLATE = app.DIK_RCONTROL

# DO NOT CHANGE ANYTHING FROM HERE ON
# author LuShan
# version : 1.1.3
# https://github.com/lushan88a/google_trans_new

LANGUAGES = {
	'af': 'afrikaans',
	'sq': 'albanian',
	'am': 'amharic',
	'ar': 'arabic',
	'hy': 'armenian',
	'az': 'azerbaijani',
	'eu': 'basque',
	'be': 'belarusian',
	'bn': 'bengali',
	'bs': 'bosnian',
	'bg': 'bulgarian',
	'ca': 'catalan',
	'ceb': 'cebuano',
	'ny': 'chichewa',
	'zh-cn': 'chinese (simplified)',
	'zh-tw': 'chinese (traditional)',
	'co': 'corsican',
	'hr': 'croatian',
	'cs': 'czech',
	'da': 'danish',
	'nl': 'dutch',
	'en': 'english',
	'eo': 'esperanto',
	'et': 'estonian',
	'tl': 'filipino',
	'fi': 'finnish',
	'fr': 'french',
	'fy': 'frisian',
	'gl': 'galician',
	'ka': 'georgian',
	'de': 'german',
	'el': 'greek',
	'gu': 'gujarati',
	'ht': 'haitian creole',
	'ha': 'hausa',
	'haw': 'hawaiian',
	'iw': 'hebrew',
	'he': 'hebrew',
	'hi': 'hindi',
	'hmn': 'hmong',
	'hu': 'hungarian',
	'is': 'icelandic',
	'ig': 'igbo',
	'id': 'indonesian',
	'ga': 'irish',
	'it': 'italian',
	'ja': 'japanese',
	'jw': 'javanese',
	'kn': 'kannada',
	'kk': 'kazakh',
	'km': 'khmer',
	'ko': 'korean',
	'ku': 'kurdish (kurmanji)',
	'ky': 'kyrgyz',
	'lo': 'lao',
	'la': 'latin',
	'lv': 'latvian',
	'lt': 'lithuanian',
	'lb': 'luxembourgish',
	'mk': 'macedonian',
	'mg': 'malagasy',
	'ms': 'malay',
	'ml': 'malayalam',
	'mt': 'maltese',
	'mi': 'maori',
	'mr': 'marathi',
	'mn': 'mongolian',
	'my': 'myanmar (burmese)',
	'ne': 'nepali',
	'no': 'norwegian',
	'or': 'odia',
	'ps': 'pashto',
	'fa': 'persian',
	'pl': 'polish',
	'pt': 'portuguese',
	'pa': 'punjabi',
	'ro': 'romanian',
	'ru': 'russian',
	'sm': 'samoan',
	'gd': 'scots gaelic',
	'sr': 'serbian',
	'st': 'sesotho',
	'sn': 'shona',
	'sd': 'sindhi',
	'si': 'sinhala',
	'sk': 'slovak',
	'sl': 'slovenian',
	'so': 'somali',
	'es': 'spanish',
	'su': 'sundanese',
	'sw': 'swahili',
	'sv': 'swedish',
	'tg': 'tajik',
	'ta': 'tamil',
	'tt': 'tatar',
	'te': 'telugu',
	'th': 'thai',
	'tr': 'turkish',
	'tk': 'turkmen',
	'uk': 'ukrainian',
	'ur': 'urdu',
	'ug': 'uyghur',
	'uz': 'uzbek',
	'vi': 'vietnamese',
	'cy': 'welsh',
	'xh': 'xhosa',
	'yi': 'yiddish',
	'yo': 'yoruba',
	'zu': 'zulu',
}

DEFAULT_SERVICE_URLS = ('translate.google.ac','translate.google.ad','translate.google.ae',
						'translate.google.al','translate.google.am','translate.google.as',
						'translate.google.at','translate.google.az','translate.google.ba',
						'translate.google.be','translate.google.bf','translate.google.bg',
						'translate.google.bi','translate.google.bj','translate.google.bs',
						'translate.google.bt','translate.google.by','translate.google.ca',
						'translate.google.cat','translate.google.cc','translate.google.cd',
						'translate.google.cf','translate.google.cg','translate.google.ch',
						'translate.google.ci','translate.google.cl','translate.google.cm',
						'translate.google.cn','translate.google.co.ao','translate.google.co.bw',
						'translate.google.co.ck','translate.google.co.cr','translate.google.co.id',
						'translate.google.co.il','translate.google.co.in','translate.google.co.jp',
						'translate.google.co.ke','translate.google.co.kr','translate.google.co.ls',
						'translate.google.co.ma','translate.google.co.mz','translate.google.co.nz',
						'translate.google.co.th','translate.google.co.tz','translate.google.co.ug',
						'translate.google.co.uk','translate.google.co.uz','translate.google.co.ve',
						'translate.google.co.vi','translate.google.co.za','translate.google.co.zm',
						'translate.google.co.zw','translate.google.co','translate.google.com.af',
						'translate.google.com.ag','translate.google.com.ai','translate.google.com.ar',
						'translate.google.com.au','translate.google.com.bd','translate.google.com.bh',
						'translate.google.com.bn','translate.google.com.bo','translate.google.com.br',
						'translate.google.com.bz','translate.google.com.co','translate.google.com.cu',
						'translate.google.com.cy','translate.google.com.do','translate.google.com.ec',
						'translate.google.com.eg','translate.google.com.et','translate.google.com.fj',
						'translate.google.com.gh','translate.google.com.gi','translate.google.com.gt',
						'translate.google.com.hk','translate.google.com.jm','translate.google.com.kh',
						'translate.google.com.kw','translate.google.com.lb','translate.google.com.lc',
						'translate.google.com.ly','translate.google.com.mm','translate.google.com.mt',
						'translate.google.com.mx','translate.google.com.my','translate.google.com.na',
						'translate.google.com.ng','translate.google.com.ni','translate.google.com.np',
						'translate.google.com.om','translate.google.com.pa','translate.google.com.pe',
						'translate.google.com.pg','translate.google.com.ph','translate.google.com.pk',
						'translate.google.com.pr','translate.google.com.py','translate.google.com.qa',
						'translate.google.com.sa','translate.google.com.sb','translate.google.com.sg',
						'translate.google.com.sl','translate.google.com.sv','translate.google.com.tj',
						'translate.google.com.tr','translate.google.com.tw','translate.google.com.ua',
						'translate.google.com.uy','translate.google.com.vc','translate.google.com.vn',
						'translate.google.com','translate.google.cv','translate.google.cx',
						'translate.google.cz','translate.google.de','translate.google.dj',
						'translate.google.dk','translate.google.dm','translate.google.dz',
						'translate.google.ee','translate.google.es','translate.google.eu',
						'translate.google.fi','translate.google.fm','translate.google.fr',
						'translate.google.ga','translate.google.ge','translate.google.gf',
						'translate.google.gg','translate.google.gl','translate.google.gm',
						'translate.google.gp','translate.google.gr','translate.google.gy',
						'translate.google.hn','translate.google.hr','translate.google.ht',
						'translate.google.hu','translate.google.ie','translate.google.im',
						'translate.google.io','translate.google.iq','translate.google.is',
						'translate.google.it','translate.google.je','translate.google.jo',
						'translate.google.kg','translate.google.ki','translate.google.kz',
						'translate.google.la','translate.google.li','translate.google.lk',
						'translate.google.lt','translate.google.lu','translate.google.lv',
						'translate.google.md','translate.google.me','translate.google.mg',
						'translate.google.mk','translate.google.ml','translate.google.mn',
						'translate.google.ms','translate.google.mu','translate.google.mv',
						'translate.google.mw','translate.google.ne','translate.google.nf',
						'translate.google.nl','translate.google.no','translate.google.nr',
						'translate.google.nu','translate.google.pl','translate.google.pn',
						'translate.google.ps','translate.google.pt','translate.google.ro',
						'translate.google.rs','translate.google.ru','translate.google.rw',
						'translate.google.sc','translate.google.se','translate.google.sh',
						'translate.google.si','translate.google.sk','translate.google.sm',
						'translate.google.sn','translate.google.so','translate.google.sr',
						'translate.google.st','translate.google.td','translate.google.tg',
						'translate.google.tk','translate.google.tl','translate.google.tm',
						'translate.google.tn','translate.google.to','translate.google.tt',
						'translate.google.us','translate.google.vg','translate.google.vu','translate.google.ws')

log = logging.getLogger(__name__)
log.addHandler(logging.NullHandler())

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

URLS_SUFFIX = [re.search('translate.google.(.*)',url.strip()).group(1) for url in DEFAULT_SERVICE_URLS]
URL_SUFFIX_DEFAULT = 'cn'

class google_new_transError(Exception):
	"""Exception that uses context to present a meaningful error message"""

	def __init__(self, msg=None, **kwargs):
		self.tts = kwargs.pop('tts', None)
		self.rsp = kwargs.pop('response', None)
		if msg:
			self.msg = msg
		elif self.tts is not None:
			self.msg = self.infer_msg(self.tts, self.rsp)
		else:
			self.msg = None
		super(google_new_transError, self).__init__(self.msg)

	def infer_msg(self, tts, rsp=None):
		cause = "Unknown"

		if rsp is None:
			premise = "Failed to connect"

			return "{}. Probable cause: {}".format(premise, "timeout")
			# if tts.tld != 'com':
			#     host = _translate_url(tld=tts.tld)
			#     cause = "Host '{}' is not reachable".format(host)

		else:
			status = rsp.status_code
			reason = rsp.reason

			premise = "{:d} ({}) from TTS API".format(status, reason)

			if status == 403:
				cause = "Bad token or upstream API changes"
			elif status == 200 and not tts.lang_check:
				cause = "No audio stream in response. Unsupported language '%s'" % self.tts.lang
			elif status >= 500:
				cause = "Uptream API error. Try again later."

		return "{}. Probable cause: {}".format(premise, cause)

class google_translator:
	'''
	You can use 108 language in target and source,details view LANGUAGES.
	Target language: like 'en'、'zh'、'th'...
	:param url_suffix: The source text(s) to be translated. Batch translation is supported via sequence input.
					   The value should be one of the url_suffix listed in : `DEFAULT_SERVICE_URLS`
	:type url_suffix: UTF-8 :class:`str`; :class:`unicode`; string sequence (list, tuple, iterator, generator)
	:param text: The source text(s) to be translated.
	:type text: UTF-8 :class:`str`; :class:`unicode`;
	:param lang_tgt: The language to translate the source text into.
					 The value should be one of the language codes listed in : `LANGUAGES`
	:type lang_tgt: :class:`str`; :class:`unicode`
	:param lang_src: The language of the source text.
					The value should be one of the language codes listed in :const:`googletrans.LANGUAGES`
					If a language is not specified,
					the system will attempt to identify the source language automatically.
	:type lang_src: :class:`str`; :class:`unicode`
	:param timeout: Timeout Will be used for every request.
	:type timeout: number or a double of numbers
	:param proxies: proxies Will be used for every request.
	:type proxies: class : dict; like: {'http': 'http:171.112.169.47:19934/', 'https': 'https:171.112.169.47:19934/'}
	'''

	def __init__(self, url_suffix="cn", timeout=5, proxies=None):
		self.proxies = proxies
		if url_suffix not in URLS_SUFFIX:
			self.url_suffix = URL_SUFFIX_DEFAULT
		else:
			self.url_suffix = url_suffix
		url_base = "https://translate.google.{}".format(self.url_suffix)
		self.url = url_base + "/_/TranslateWebserverUi/data/batchexecute"
		self.timeout = timeout

	def _package_rpc(self, text, lang_src='auto', lang_tgt='auto'):
		GOOGLE_TTS_RPC = ["MkEWBc"]
		parameter = [[text.strip(), lang_src, lang_tgt, True], [1]]
		escaped_parameter = json.dumps(parameter, separators=(',', ':'))
		rpc = [[[random.choice(GOOGLE_TTS_RPC), escaped_parameter, None, "generic"]]]
		espaced_rpc = json.dumps(rpc, separators=(',', ':'))
		# text_urldecode = quote(text.strip())
		freq_initial = "f.req={}&".format(urllib.quote(espaced_rpc))
		freq = freq_initial
		return freq

	def translate(self, text, lang_tgt='auto', lang_src='auto', pronounce=False):
		try:
			lang = LANGUAGES[lang_src]
		except:
			lang_src = 'auto'
		try:
			lang = LANGUAGES[lang_tgt]
		except:
			lang_src = 'auto'
		text = str(text)
		if len(text) >= 5000:
			return "Warning: Can only detect less than 5000 characters"
		if len(text) == 0:
			return ""
		headers = {
			"Referer": "http://translate.google.{}/".format(self.url_suffix),
			"User-Agent":
				"Mozilla/5.0 (Windows NT 10.0; WOW64) "
				"AppleWebKit/537.36 (KHTML, like Gecko) "
				"Chrome/47.0.2526.106 Safari/537.36",
			"Content-Type": "application/x-www-form-urlencoded;charset=utf-8"
		}
		freq = self._package_rpc(text, lang_src, lang_tgt)
		response = requests.Request(method='POST',
									url=self.url,
									data=freq,
									headers=headers,
									)
		try:
			if self.proxies == None or type(self.proxies) != dict:
				self.proxies = {}
			with requests.Session() as s:
				s.proxies = self.proxies
				r = s.send(request=response.prepare(),
						   verify=False,
						   timeout=self.timeout)
			for line in r.iter_lines(chunk_size=1024):
				decoded_line = line.decode('utf-8')
				if "MkEWBc" in decoded_line:
					try:
						response = decoded_line
						response = json.loads(response)
						response = list(response)
						response = json.loads(response[0][2])
						response_ = list(response)
						response = response_[1][0]
						if len(response) == 1:
							if len(response[0]) > 5:
								sentences = response[0][5]
							else: ## only url
								sentences = response[0][0]
								if pronounce == False:
									return sentences
								elif pronounce == True:
									return [sentences,None,None]
							translate_text = ""
							for sentence in sentences:
								sentence = sentence[0]
								translate_text += sentence.strip() + ' '
							translate_text = translate_text
							if pronounce == False:
								return translate_text
							elif pronounce == True:
								pronounce_src = (response_[0][0])
								pronounce_tgt = (response_[1][0][0][1])
								return [translate_text, pronounce_src, pronounce_tgt]
						elif len(response) == 2:
							sentences = []
							for i in response:
								sentences.append(i[0])
							if pronounce == False:
								return sentences
							elif pronounce == True:
								pronounce_src = (response_[0][0])
								pronounce_tgt = (response_[1][0][0][1])
								return [sentences, pronounce_src, pronounce_tgt]
					except Exception as e:
						raise e
			r.raise_for_status()
		except requests.exceptions.ConnectTimeout as e:
			raise e
		except requests.exceptions.HTTPError as e:
			# Request successful, bad response
			raise google_new_transError(tts=self, response=r)
		except requests.exceptions.RequestException as e:
			# Request failed
			raise google_new_transError(tts=self)

	def detect(self, text):
		text = str(text)
		if len(text) >= 5000:
			return log.debug("Warning: Can only detect less than 5000 characters")
		if len(text) == 0:
			return ""
		headers = {
			"Referer": "http://translate.google.{}/".format(self.url_suffix),
			"User-Agent":
				"Mozilla/5.0 (Windows NT 10.0; WOW64) "
				"AppleWebKit/537.36 (KHTML, like Gecko) "
				"Chrome/47.0.2526.106 Safari/537.36",
			"Content-Type": "application/x-www-form-urlencoded;charset=utf-8"
		}
		freq = self._package_rpc(text)
		response = requests.Request(method='POST',
									url=self.url,
									data=freq,
									headers=headers)
		try:
			if self.proxies == None or type(self.proxies) != dict:
				self.proxies = {}
			with requests.Session() as s:
				s.proxies = self.proxies
				r = s.send(request=response.prepare(),
						   verify=False,
						   timeout=self.timeout)

			for line in r.iter_lines(chunk_size=1024):
				decoded_line = line.decode('utf-8')
				if "MkEWBc" in decoded_line:
					# regex_str = r"\[\[\"wrb.fr\",\"MkEWBc\",\"\[\[(.*).*?,\[\[\["
					try:
						# data_got = re.search(regex_str,decoded_line).group(1)
						response = (decoded_line)
						response = json.loads(response)
						response = list(response)
						response = json.loads(response[0][2])
						response = list(response)
						detect_lang = response[0][2]
					except Exception:
						raise Exception
					# data_got = data_got.split('\\\"]')[0]
					return [detect_lang, LANGUAGES[detect_lang.lower()]]
			r.raise_for_status()
		except requests.exceptions.HTTPError as e:
			# Request successful, bad response
			log.debug(str(e))
			raise google_new_transError(tts=self, response=r)
		except requests.exceptions.RequestException as e:
			# Request failed
			log.debug(str(e))
			raise google_new_transError(tts=self)

TRANSLATOR_INSTANCE = google_translator() 

class TextTranslator(object):
	def __init__(self):
		self.isMultiText = False
		self.textToTranslate = ""
		self.translatedTextDict = {}
		self.lastTranslatedText = ""
		self.wrapper = textwrap.TextWrapper(width=50) 
	
		toolTipTranslation = uiToolTip.ToolTip(230)
		toolTipTranslation.HideToolTip()
		self.toolTipTranslation = toolTipTranslation

		self.waitingDialog = WaitingDialog()

	def TranslateTextAvailable(self):
		if self.textToTranslate != "":
			return True
		else:
			return False
		
	def SetTranslateText(self, text):
		if text:
			pos = text.find(' : ') ## filter chat names
			if pos != -1:
				text = text[pos+3:]
			
			self.isMultiText = False
			self.textToTranslate = text
				
	def TranslateText(self):
		toolTipShowTime = TOOLTIP_SHOW_TIME
		if self.isMultiText:
			toolTipShowTime = TOOLTIP_MULTI_TEXT_SHOW_TIME

		self.isMultiText = False

		if self.lastTranslatedText != self.textToTranslate:
			if self.textToTranslate:
				textToTranslate = self.textToTranslate
				self.textToTranslate = ""
				langTo = systemSetting.GetTransLangKey()
				if langTo not in self.translatedTextDict.keys():
					self.translatedTextDict[langTo] = {}
			
				if textToTranslate not in self.translatedTextDict[langTo].keys():
					self.translatedTextDict[langTo][textToTranslate] = {'translation':"...", 'srcLang':"...", 'destLang':"..."}
					self.Translate(textToTranslate)

				self.toolTipTranslation.ClearToolTip()

				wordListTextToTranslate = self.wrapper.wrap(text=textToTranslate)
				for word in wordListTextToTranslate:
					self.toolTipTranslation.AppendTextLine(word, COLOR_TEXT)

				wordListTranslation = self.wrapper.wrap(text=self.translatedTextDict[langTo][textToTranslate]['translation'])
				self.toolTipTranslation.AppendTextLine("[%s -> %s]" % (self.translatedTextDict[langTo][textToTranslate]['srcLang'], self.translatedTextDict[langTo][textToTranslate]['destLang']), COLOR_INFO)
				for word in wordListTranslation:
					self.toolTipTranslation.AppendTextLine(word, COLOR_TEXT)
		self.ShowToolTip(toolTipShowTime)
	
	def AppendTranslateText(self, text):
		self.isMultiText = True
		if text and not text in self.textToTranslate:
			self.textToTranslate += " "+text
			self.RefreshToolTipMultiTranslateText()

	def RefreshToolTipMultiTranslateText(self):
		self.toolTipTranslation.ClearToolTip()
		wordList = self.wrapper.wrap(text=self.textToTranslate)
		
		for word in wordList:
			self.toolTipTranslation.AppendTextLine(word, COLOR_TEXT)
		self.toolTipTranslation.AppendTextLine(localeInfo.GTRANS_LEAVE_SHIFT_TO_TRANSLATE, COLOR_INFO)
		self.ShowToolTip(TOOLTIP_MULTI_TEXT_SHOW_TIME)

	def Translate(self, textToTranslate):
		try:
			langTo = systemSetting.GetTransLangKey()

			translated = TRANSLATOR_INSTANCE.translate(textToTranslate.decode("cp%s" % app.GetDefaultCodePage()).encode('utf-8', errors='replace'), lang_tgt=langTo)
		
			if langTo in constInfo.AVAILABLE_LANGUAGES:
				encoding = constInfo.AVAILABLE_LANGUAGES[langTo]['encoding']
			else:
				encoding = DEFAULT_LANG_TO_ENCODING

			srcLang = TRANSLATOR_INSTANCE.detect(unicode(textToTranslate, errors='replace'))[0]

			if srcLang in constInfo.AVAILABLE_LANGUAGES:
				srcLang = constInfo.AVAILABLE_LANGUAGES[srcLang]['name']

			self.translatedTextDict[langTo][textToTranslate] = {
				'translation' : translated.encode(encoding, errors='replace'), 
				'srcLang' : srcLang,
				'destLang' : langTo
			}

		except Exception as e:
			print(e)

	def ShowToolTip(self, time = 0.0):
		self.toolTipTranslation.Show()
		if time != 0.0:
			self.waitingDialog.Open(time)
			self.waitingDialog.SAFE_SetTimeOverEvent(self.HideToolTip)
		else:
			self.waitingDialog.Stop()

	def HideToolTip(self):
		if self.toolTipTranslation.IsShow():
			self.toolTipTranslation.HideToolTip()
			self.textToTranslate = ""

	def OnKeyDownEvent(self, key):
		if key == KEY_TO_TRANSLATE:
			if app.TRANS != app.GetCursor():
				app.SetCursor(app.TRANS)
			return True
		return False
	
	def OnKeyUpEvent(self, key):
		if key == KEY_TO_TRANSLATE:
			if app.TRANS == app.GetCursor():
				app.SetCursor(app.NORMAL)
				return True
		return False

class TranslatorBoard(ui.BoardWithTitleBar):
	def __init__(self, editLine):
		ui.BoardWithTitleBar.__init__(self)
		self.SetSize(200+30, 90)
		self.AddFlag('float')
		self.SetTitleName(localeInfo.GTRANS_TITLE)
		self.SetCloseEvent(self.Close)
		self.SetCenterPosition()
		self.Hide()

		self.editLine = editLine

		comboBoxLanguageFrom = ui.ComboBox()
		comboBoxLanguageFrom.SetParent(self)
		comboBoxLanguageFrom.SetPosition(15,30+25)
		comboBoxLanguageFrom.SetSize(50, 20)
		comboBoxLanguageFrom.ClearItem()

		comboBoxLanguageTo = ui.ComboBox()
		comboBoxLanguageTo.SetParent(self)
		comboBoxLanguageTo.SetPosition(15+60,30+25)
		comboBoxLanguageTo.SetSize(50, 20)
		comboBoxLanguageTo.ClearItem()

		comboBoxLanguageFrom.InsertItem('auto', 'auto')
		for langKey, lang in constInfo.AVAILABLE_LANGUAGES.iteritems():
			comboBoxLanguageFrom.InsertItem(langKey, lang['name'])
			comboBoxLanguageTo.InsertItem(langKey, lang['name'])

		self.languageFrom = 'auto' 
		comboBoxLanguageFrom.SetCurrentItem("auto")

		self.languageTo = list(constInfo.AVAILABLE_LANGUAGES)[0]
		comboBoxLanguageTo.SetCurrentItem(constInfo.AVAILABLE_LANGUAGES[self.languageTo]['name'])
		
		self.comboBoxLanguageFrom = comboBoxLanguageFrom
		self.comboBoxLanguageFrom.SetEvent(self._OnSelectItemComboBoxLanguageFrom)

		self.comboBoxLanguageTo = comboBoxLanguageTo
		self.comboBoxLanguageTo.SetEvent(self._OnSelectItemComboBoxLanguageTo)

		self.comboBoxLanguageFrom.Show()
		self.comboBoxLanguageTo.Show()

		buttonTranslate = ui.Button()
		buttonTranslate.SetParent(self)
		buttonTranslate.SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
		buttonTranslate.SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
		buttonTranslate.SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
		buttonTranslate.SetText(localeInfo.GTRANS_TRANSLATE_BOARD)
		buttonTranslate.SetPosition(260-88-42,30+25)
		buttonTranslate.SetEvent(self._OnClickButtonTranslate)
		buttonTranslate.Show()
		self.buttonTranslate = buttonTranslate

		buttonInfo = ui.Button()
		buttonInfo.SetParent(self)
		buttonInfo.SetUpVisual("d:/ymir work/ui/public/small_button_01.sub")
		buttonInfo.SetOverVisual("d:/ymir work/ui/public/small_button_02.sub")
		buttonInfo.SetDownVisual("d:/ymir work/ui/public/small_button_03.sub")
		buttonInfo.SetText(TEXT_BUTTON_INFO)
		buttonInfo.SetPosition(260-43-42,30)
		buttonInfo.SetEvent(self._OnClickButtonInfo)
		if SHOW_INFO_BUTTON_ON_BOARD:
			buttonInfo.Show()
		self.buttonInfo = buttonInfo

		checkBox = ui.CheckBox()
		checkBox.SetParent(self)
		checkBox.SetPosition(15,30+5)
		checkBox.SetCheckStatus(False)
		checkBox.SetTextInfo(localeInfo.GTRANS_INSTANT_TRANSLATE)
		checkBox.Show()
		self.checkBox = checkBox

		toolTipInfo = uiToolTip.ToolTip(230)
		toolTipInfo.HideToolTip()
		self.toolTipInfo = toolTipInfo

		self.wrapper = textwrap.TextWrapper(width=50)
		self.wordListInfoTranslatorUsage = self.wrapper.wrap(text=localeInfo.GTRANS_INFO_TRANSLATOR_USAGE)
		self.waitingDialog = WaitingDialog()

	def _OnClickButtonInfo(self):
		self.toolTipInfo.ClearToolTip()
		self.toolTipInfo.AppendTextLine(localeInfo.GTRANS_INFO_TRANSLATOR, COLOR_INFO)
		for word in self.wordListInfoTranslatorUsage:
			self.toolTipInfo.AppendTextLine(word, COLOR_TEXT)

		self.ShowToolTip(TOOLTIP_INFO_SHOW_TIME)

	def ShowToolTip(self, time = 0.0):
		self.toolTipInfo.Show()
		if time != 0.0:
			self.waitingDialog.Open(time)
			self.waitingDialog.SAFE_SetTimeOverEvent(self.HideToolTip)
		else:
			self.waitingDialog.Stop()

	def HideToolTip(self):
		if self.toolTipInfo.IsShow():
			self.toolTipInfo.HideToolTip()

	def __del__(self):
		self.Close()
		ui.BoardWithTitleBar.__del__(self)

	def Close(self):
		self.Hide()

	def SetWhisperTitleName(self, targetName):
		self.SetTitleName(localeInfo.GTRANS_WHISPER_TITLE_NAME % targetName)

	def SetChatTitleName(self):
		self.SetTitleName(localeInfo.GTRANS_CHAT_TITLE_NAME)

	def _OnSelectItemComboBoxLanguageFrom(self, id):
		if id == 'auto':
			self.comboBoxLanguageFrom.SetCurrentItem("auto")
		else:
			self.comboBoxLanguageFrom.SetCurrentItem(constInfo.AVAILABLE_LANGUAGES[id]['name'])
		self.languageFrom = id

	def _OnSelectItemComboBoxLanguageTo(self, id):
		self.comboBoxLanguageTo.SetCurrentItem(constInfo.AVAILABLE_LANGUAGES[id]['name'])
		self.languageTo = id

	def InitiateTranslation(self):
		if self.checkBox.GetCheckStatus() == True:
			self.__Translate()

	def _OnClickButtonTranslate(self):
		self.__Translate()
		self.Close()

	def __Translate(self):
		textToTranslate = self.editLine.GetText()
		if textToTranslate:
			chatMark = ""
			for c in ['!','#','%','/n ','/notice ','/notice_map ','/notice_test ','/b ','/big_notice ','/big_notice_map ','/big_notice_test ']:
				if textToTranslate[0:len(c)] == c:
					chatMark = c
					textToTranslate = textToTranslate[len(c):]
					break
			if not chatMark:
				for b in ['/']:
					if textToTranslate[0:len(b)] == b:
						return
			startPositions = [(i.start()) for i in re.finditer('\|cff', textToTranslate)]
			replaceTexts = []
			for startPos in startPositions:
				endPos = textToTranslate[startPos:].find("|h|r")
				if (endPos != -1):
					replaceTexts.append(textToTranslate[startPos:endPos+startPos+4])

			for replaceText in replaceTexts:
				textToTranslate = textToTranslate.replace(replaceText, "%s")
			
			try:
				if self.languageFrom == 'auto':
					translated = TRANSLATOR_INSTANCE.translate(textToTranslate.decode("cp%s" % app.GetDefaultCodePage()).encode('utf-8', errors='replace'), lang_tgt=self.languageTo)
				else:
					translated = TRANSLATOR_INSTANCE.translate(textToTranslate.decode("cp%s" % app.GetDefaultCodePage()).encode('utf-8', errors='replace'), lang_src=self.languageFrom, lang_tgt=self.languageTo)
					
				if self.languageTo in constInfo.AVAILABLE_LANGUAGES:
					encoding = constInfo.AVAILABLE_LANGUAGES[self.languageTo]['encoding']
				else:
					encoding = DEFAULT_LANG_TO_ENCODING

				translatedText = translated.encode(encoding, errors='replace')
				self.editLine.SetText(chatMark + (translatedText %tuple(replaceTexts)))
			except Exception as e:
				print(e)
	
	def OpenTranslator(self):
		self.SetTop()
		self.Show()

class WaitingDialog(ui.Window):
	def __init__(self):
		ui.Window.__init__(self)
		self.eventTimeOver = lambda *arg: None
		self.eventExit = lambda *arg: None
		self.run = False

	def Open(self, waitTime):
		curTime = time.clock()
		self.endTime = curTime + waitTime
		self.run = True
		self.Show()	

	def Stop(self):
		self.run = False

	def Close(self):
		self.Hide()

	def Destroy(self):
		self.Hide()

	def SAFE_SetTimeOverEvent(self, event):
		self.eventTimeOver = ui.__mem_func__(event)

	def SAFE_SetExitEvent(self, event):
		self.eventExit = ui.__mem_func__(event)
		
	def OnUpdate(self):
		if self.run:
			lastTime = max(0, self.endTime - time.clock())
			if 0 == lastTime:
				self.Close()
				self.eventTimeOver()

wnd = TextTranslator()