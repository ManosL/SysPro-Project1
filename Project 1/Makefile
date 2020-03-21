COMPILER = gcc
FLAGS    = -g -g3 -Wpedantic
OBJS     =  main.o DiseaseMonitor.o AVLTree.o RecPtrLinkedList.o Date.o MaxHeap.o HashFunctions.o MathOps.o StringUtils.o RecordsBucketList.o RecordsHashTable.o BucketList.o StaticHashTable.o Record.o
diseaseMonitor : $(OBJS)
	$(COMPILER) $(FLAGS) $(OBJS) -o diseaseMonitor

main.o: ./MainFiles/main.c
	$(COMPILER) $(FLAGS) -c ./MainFiles/main.c

DiseaseMonitor.o: ./MainFiles/DiseaseMonitor/DiseaseMonitor.c
	$(COMPILER) $(FLAGS) -c ./MainFiles/DiseaseMonitor/DiseaseMonitor.c
	
AVLTree.o: ./AVLTree/AVLTree.c
	$(COMPILER) $(FLAGS) -c ./AVLTree/AVLTree.c

RecPtrLinkedList.o: ./AVLTree/RecPtrLinkedList/RecPtrLinkedList.c
	$(COMPILER) $(FLAGS) -c ./AVLTree/RecPtrLinkedList/RecPtrLinkedList.c

Date.o: ./Date/Date.c
	$(COMPILER) $(FLAGS) -c ./Date/Date.c

MaxHeap.o: ./MaxHeap/MaxHeap.c
	$(COMPILER) $(FLAGS) -c ./MaxHeap/MaxHeap.c

HashFunctions.o: ./Misc/HashFunctions.c
	$(COMPILER) $(FLAGS) -c ./Misc/HashFunctions.c

MathOps.o: ./Misc/MathOps.c
	$(COMPILER) $(FLAGS) -c ./Misc/MathOps.c

StringUtils.o: ./Misc/StringUtils.c
	$(COMPILER) $(FLAGS) -c ./Misc/StringUtils.c
	
RecordsBucketList.o: ./RecordsHashTable/RecordsBucketList/RecordsBucketList.c
	$(COMPILER) $(FLAGS) -c ./RecordsHashTable/RecordsBucketList/RecordsBucketList.c

RecordsHashTable.o: ./RecordsHashTable/RecordsHashTable.c
	$(COMPILER) $(FLAGS) -c ./RecordsHashTable/RecordsHashTable.c

BucketList.o: ./StaticHashTable/BucketList/BucketList.c
	$(COMPILER) $(FLAGS) -c ./StaticHashTable/BucketList/BucketList.c

StaticHashTable.o: ./StaticHashTable/StaticHashTable.c
	$(COMPILER) $(FLAGS) -c ./StaticHashTable/StaticHashTable.c

Record.o: ./Record/Record.c
	$(COMPILER) $(FLAGS) -c ./Record/Record.c
	
clean:
	rm -f $(OBJS) diseaseMonitor