import ast
import requests

def getResponseFromServer(gameRep, turnMode):
	board=[]
	for i in gameRep['ringsB']:
	    board.append((i,'Ring B'))
	for i in gameRep['ringsA']:
	    board.append((i,'Ring W'))
	for i in gameRep['markersB']:
	    board.append((i,'Marker B'))
	for i in gameRep['markersA']:
	    board.append((i,'Marker A'))

	board=(str(board)).replace("'","")


	query_string='GameState {activePlayer = W, turnMode = '+turnMode+', board = Board {bmap = fromList '
	query_string+=str(board)
	query_string+=', ringsB ='+ str(gameRep['ringsB'])
	query_string+=', ringsW = '+ str(gameRep['ringsA'])
	query_string+=', markersB = '+ str(gameRep['markersB'])
	query_string+=', markersW = '+ str(gameRep['markersA'])
	pointsB,pointsA=0,0
	if(turnMode!='AddRing'):
	    pointsB=5-len(gameRep['ringsB'])
	    pointsA=5-len(gameRep['ringsA'])
	    
	query_string+='}, pointsB = '+str(pointsB)+', pointsW = '+str(pointsA)+'}'

	# Send GET request with query_string, and return parsed response

	url='https://yinsh-backend.herokuapp.com/'

	# payload={'gamestate':'GameState {activePlayer = W, turnMode = AddRing, board = Board {bmap = fromList [((1,2),Ring B)], ringsB = [(1,2)], ringsW = [], markersB = [], markersW = []}, pointsB = 0, pointsW = 0}'}
	payload={'gamestate':query_string}
	response=(requests.get(url,params=payload)).text

	print('# query_string: '+query_string)
	print('# Server response: '+response)

	nextGameRep={}
	response=response.split('ringsB = ')[1]
	nextGameRep['ringsB']=ast.literal_eval( (response.split('],')[0])+']' )

	response=response.split('ringsW = ')[1]
	nextGameRep['ringsA']=ast.literal_eval( (response.split('],')[0])+']' )

	response=response.split('markersB = ')[1]
	nextGameRep['markersB']=ast.literal_eval( (response.split('],')[0])+']' )

	response=response.split('markersW = ')[1]
	nextGameRep['markersA']=ast.literal_eval( (response.split(']},')[0])+']' )

	return nextGameRep

print(getResponseFromServer({'ringsB':[(1,2)],'ringsA':[],'markersB':[],'markersA':[]},'AddRing'))





