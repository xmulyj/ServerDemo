all:
	(cd NameSvr && $(MAKE) all)
	(cd LogicSvr && $(MAKE) all)
	(cd AccessSvr && $(MAKE) all)


clean:
	(cd NameSvr && $(MAKE) clean)
	(cd LogicSvr && $(MAKE) clean)
	(cd AccessSvr && $(MAKE) clean)
