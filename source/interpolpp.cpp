
		/*
			elIndex=resultmodel->getElementIndex(resultmodel->nodeList[k].adjacentElements[e]);
				vNum=resultmodel->elementList[elIndex].getVerticeNumber(resultmodel->nodeList[k].getLabel());
				if (vNum<0)
				{
					emit sendMessage("Error in NodeElConnectivity");
					return;
				
				}
				//sendMessage("Possible elements for node %1 : 
				//Find the adjoining edges
				switch (vNum)
				{
					case 0:
						//01,02,04
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[2])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[4])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0]);
						n3length=n3.norm();
					break;
					case 1:
					//10,13,15
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[3])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[5])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n3length=n3.norm();
					break;
					case 2:
					//20,23,26
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[3])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[5])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n3length=n3.norm();
					break;
					case 3:
					//31,32,37
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[3])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[5])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n3length=n3.norm();
					break;
					case 4:
					//40,45,46
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[3])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[5])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n3length=n3.norm();
					break;
					case 5:
					//54,51,57
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[3])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[5])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n3length=n3.norm();
					break;
					case 6:
					//62,64,67
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[3])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[5])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n3length=n3.norm();
					break;
					case 7:
					//73,75,76	
						n1=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[0])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n1length=n1.norm();
						n2=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[3])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n2length=n2.norm();
						n3=resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[5])-resultmodel->getNodeCoords(resultmodel->elementList[elIndex].nodeList[1]);
						n3length=n3.norm();
					break;
				}	
					//calculate the distances
					d1=nodeList[n].distanceToPlane(origin,n1);
					d2=nodeList[n].distanceToPlane(origin,n2);
					d3=nodeList[n].distanceToPlane(origin,n3);
		}
		break;
	}
	*/