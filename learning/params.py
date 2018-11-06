def init():
    #path
    global logs_dir,games_dir
    logs_dir='./logs/run1'
    games_dir='./logs/run1'
    
    global lamda,learning_rate,num_epochs
    lamda=0.1
    learning_rate=0.001
    num_epochs=100

    global scaling_factor #scale weights
    scaling_factor=0.0001

    global comment
    comment="" #Comment to store logs with




paramList=[
    'n','selfPolicy','oppPolicy'
]

features=[   
    'TwoMarker',
    'ThreeMarker',
    'FourMarker',
    'FiveMarker',
    'SixOrMoreMarker',
    'TwoMarkerRing',
    'ThreeMarkerRing',
    'FourMarkerRing',
    'FiveMarkerRing',
    'SixOrMoreMarkerRing',
    'DistRing1',
    'DistRing2',
    'DistRing3',
    'DistRing4',
    'DistRing5',
    'DistRing6',
    'MarkersCount',
    'RingsCount',
    'Ring1Freedom',
    'Ring2Freedom',
    'Ring3Freedom',
    'Ring4Freedom',
    'Ring5Freedom',
    'Ring6Freedom',
    'Ring1JumpPositions',
    'Ring2JumpPositions',
    'Ring3JumpPositions',
    'Ring4JumpPositions',
    'Ring5JumpPositions',
    'Ring6JumpPositions',

    'OppTwoMarker',
    'OppThreeMarker',
    'OppFourMarker',
    'OppFiveMarker',
    'OppSixOrMoreMarker',
    'OppTwoMarkerRing',
    'OppThreeMarkerRing',
    'OppFourMarkerRing',
    'OppFiveMarkerRing',
    'OppSixOrMoreMarkerRing',
    'OppDistRing1',
    'OppDistRing2',
    'OppDistRing3',
    'OppDistRing4',
    'OppDistRing5',
    'OppDistRing6',
    'OppMarkersCount',
    'OppRingsCount',
    'OppRing1Freedom',
    'OppRing2Freedom',
    'OppRing3Freedom',
    'OppRing4Freedom',
    'OppRing5Freedom',
    'OppRing6Freedom',
    'OppRing1JumpPositions',
    'OppRing2JumpPositions',
    'OppRing3JumpPositions',
    'OppRing4JumpPositions',
    'OppRing5JumpPositions',
    'OppRing6JumpPositions',
]


