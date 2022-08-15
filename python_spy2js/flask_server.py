from selenium import webdriver
from selenium.webdriver.chrome.options import Options

from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait 
from selenium.webdriver.support import expected_conditions as EC

from PIL import Image
from imgcompare import image_diff_percent

import sys, os

options = Options()
options.add_argument('--user-agent="Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0"')
#options.add_argument('--headless')
#options.add_argument('https.response.timeout=1')
options.add_argument('window-size=1920,1080')
options.add_argument("window-position=-10000,0")

driver = None

def pil_per(f1, img2) :
	img1 = Image.open(f1)
	w1, h1 = img1.size
	w2, h2 = img2.size

	if(w1!=w2 or h1!=h2) :
		newsize = (w2, h2)
		img1 = img1.resize(newsize)

	percentage = image_diff_percent(img1, img2)
	return percentage


from flask import Flask, request
app = Flask(__name__)

@app.route("/")
def running():
	return "LISTENING"

@app.route("/search")
def search():
	url = request.args.get('url')
	global driver
	if driver == None:
		driver = webdriver.Chrome(sys.argv[1],options=options)
	driver.get(url)
	try :
		element = WebDriverWait(driver, 7).until(EC.presence_of_element_located((By.CSS_SELECTOR , '#root > div.search > div.u-4\\/5-min-vh > div > main > div > div > section > ol > li > div > article > div.o-block__img')))
		# print(element.text)
		return "SUCCESS|"+driver.page_source
	except :
		return "ERROR"

@app.route("/img-compare")
def img_compare():
	img_paths = request.args.get('paths')
	img_list = img_paths.split("|")

	length = len(img_list)
	js_cover = img_list[length-1]
	js_image = Image.open(js_cover)

	for i in range(length-1) :
		spy_cover = img_list[i]
		p = pil_per(spy_cover,js_image)
		if(p<10) :
			return "SUCCESS|"+str(i)
	return "ERROR"


def kill():
	shutdown = request.environ.get('werkzeug.server.shutdown')
	if shutdown is None:
		raise RuntimeError('werkzeug server error')
	shutdown()

@app.get('/kill-server/<string:code>')
def kill_server(code):
	if(code=='COMMAND_FROM_CPP') :
		if driver :
			pid = driver.service.process.pid
			driver.quit()
			os.system("taskkill /f /pid "+str(pid))

		kill()
		return 'SUCCESS'

	return 'ERROR'

app.run(host='127.0.0.1', port=45632, threaded=True)
