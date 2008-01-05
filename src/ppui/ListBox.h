/////////////////////////////////////////////////////////////////
//
//	PPListBox control class
//
/////////////////////////////////////////////////////////////////
#ifndef LISTBOX__H
#define LISTBOX__H

#include "BasicTypes.h"
#include "Control.h"
#include "Event.h"
#include "SimpleVector.h"

// Forwards
class PPGraphicsAbstract;
class PPScrollbar;
class PPFont;
class PPButton;

class PPListBox : public PPControl, public EventListenerInterface
{
private:
	bool border;
	const PPColor* borderColor;
	const PPColor* backGroundButtonColor;
	const PPColor* textColor;

	bool editable;
	bool scrollable;
	bool autoHideVScroll;
	bool autoHideHScroll;

	bool showIndex;
	pp_int32 indexBaseCount;

	bool showSelectionAlways;
	bool selectionVisible;
	bool onlyShowIndexSelection;
	bool keepsFocus;
	bool showFocus;
	bool centerSelection;
	bool selectOnScroll;
	bool singleButtonClickEdit;
	bool allowDragSelection;
	bool rightButtonConfirm;

	PPSimpleVector<PPString>* items;
	pp_int32 startIndex;
	pp_int32 startPos;
	pp_int32 selectionIndex;
	pp_int32 columnSelectionStart;
	pp_int32 columnSelectionEnd;
	pp_int32 maxEditSize;
	pp_int32 timerTicker;
	bool lastTimerState;

	pp_int32 visibleHeight;
	pp_int32 visibleWidth;

	PPButton* backgroundButton;

	PPScrollbar* hScrollbar;
	PPScrollbar* vScrollbar;

	PPControl* caughtControl;
	bool controlCaughtByLMouseButton, controlCaughtByRMouseButton;
	bool lMouseDown, rMouseDown;

	PPFont* font;

	// UNDO
	PPString* editCopy;

	// hack
	pp_int32 lastStartIndex;
	pp_int32 lastStartPos;
	pp_int32 lastSelectionIndex;
	bool hadVScrollbar;
	bool hadHScrollbar;
	
public:
	class ColorQueryListener
	{
	public:
		virtual PPColor getColor(pp_uint32 index, PPListBox& sender) = 0;
	};
	
private:
	ColorQueryListener* colorQueryListener;

public:
	PPListBox(pp_int32 id, 
			  PPScreen* parentScreen, 
			  EventListenerInterface* eventListener, 
			  PPPoint location, 
			  PPSize size, 
			  bool border = true, 
			  bool editable = false,
			  bool scrollable = true,
			  bool showSelectionAlways = false);
	
	~PPListBox();

	void setFont(PPFont* font) { this->font = font; }
	PPFont* getFont() { return font; }

	void setBorderColor(const PPColor& color) { this->borderColor = &color; }

	void setAutoHideVScroll(bool b) { autoHideVScroll = b; }
	void setAutoHideHScroll(bool b) { autoHideHScroll = b; }

	void setShowIndex(bool showIndex);

	void showSelection(bool b) { selectionVisible = b; }
	
	void setOnlyShowIndexSelection(bool b) { onlyShowIndexSelection = b; }

	void setKeepsFocus(bool keepsFocus) { this->keepsFocus = keepsFocus; } 
	void setShowFocus(bool showFocus) { this->showFocus = showFocus; }

	void setCenterSelection(bool bCenter) { centerSelection = bCenter; }

	void setIndexBaseCount(pp_int32 indexBaseCount) { this->indexBaseCount = indexBaseCount; }

	void setSelectOnScroll(bool b) { selectOnScroll = b; }

	void setSingleButtonClickEdit(bool b) { singleButtonClickEdit = b; }
	void setAllowDragSelection(bool b) { allowDragSelection = b; }
	void setRightButtonConfirm(bool b) { rightButtonConfirm = b; }

	void setMaxEditSize(pp_int32 max) { maxEditSize = max; }

	void addItem(const PPString& item); 
	const PPString& getItem(pp_int32 index);

	void updateItem(pp_int32 index, const PPString& item);

	pp_int32 getNumItems() const { return items->size(); }

	void clear();

	pp_uint32 getSelectedIndex() const { return selectionIndex; }

	void setSelectedIndex(pp_int32 index, bool adjustStartIndex = true, bool assureCursor = true);
	void setSelectedIndexByItem(const PPString& item, bool adjustStartIndex = true); 

	bool isLastEntry() { return selectionIndex == getNumItems() - 1; }
	bool isFirstEntry() { return selectionIndex == 0; }

	bool isEditing() const { return columnSelectionStart>=0; }
	
	void placeCursorAtEnd();
	void placeCursorAtStart();

	void saveState();

	void restoreState(bool assureCursor = true);	

	// from PPControl
	virtual void paint(PPGraphicsAbstract* graphics);
	
	virtual bool gainsFocus() { return keepsFocus; }
	virtual bool gainedFocusByMouse() { return keepsFocus && ((caughtControl == NULL) && (items->size() > 0)); }

	virtual bool isActive() { return true; }
	
	virtual pp_int32 callEventListener(PPEvent* event);

	virtual void setSize(PPSize size);
	virtual void setLocation(PPPoint location);

	// from EventListenerInterface
	pp_int32 handleEvent(PPObject* sender, PPEvent* event);

	virtual bool isListBox() { return true; }

	virtual bool receiveTimerEvent() { return true; }

	void commitChanges();
	void discardChanges();
	
	void setColorQueryListener(ColorQueryListener* listener) { colorQueryListener = listener; }

private:
	enum SelectReturnCodes
	{
		SelectReturnCodeDefault,
		SelectReturnCodeBreak,
		SelectReturnCodePlaceCursor
	};

	SelectReturnCodes select(const PPPoint* p);

	void initialize();

	void calcVisible();
	void adjustScrollbarPositions();
	void adjustScrollbars();

	void assureCursorVisible();

	// new stuff
	PPRect getVisibleRect() const;	
	pp_int32 getItemHeight() const;
	pp_int32 getNumVisibleItems() const;
	pp_uint32 getMaxWidth() const;	
};

#endif