/*
 * Copyright 2016 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Implementation of a "Multi-group", similar to a Tab set but without
 * the tabs.  Control of which group is visible is one of several methods
 *  that select the group to be displayed.
 */

#include "Fl_MultiGroup.H"
#include <FL/Fl_Window.H>

Fl_MultiGroup::Fl_MultiGroup (int x, int y, int w, int h, const char *l)
  : Fl_Group(x, y, w, h, l) 
{
  childStack = new Fl_Widget*[8];
  stackSize = 8;
  numChildren = 0;
  top = -1;
  selectedChild = 0;
  begin();
}

bool Fl_MultiGroup::selectChild (int which)
{
  if (which < 0 || which >= children())
    return false;
  
  // Hide the current selected
  child(selectedChild)->hide();
  // Select the new child
  selectedChild = which;
  child(which)->show();
  window()->redraw();
  return true;
}

bool Fl_MultiGroup::selectChild (Fl_Widget *w)
{
  int which = find(w);
  if (which < children())
    return selectChild(which);
  else
    return false;
}

bool Fl_MultiGroup::pushChild (int which)
{
  if (which < 0 || which >= children())
    return false;
  top++;
  if (top == stackSize) {
    int i;
    Fl_Widget **newStack;
    newStack = new Fl_Widget*[2*stackSize];
    for (i=0; i < stackSize; i++)
      newStack[i] = childStack[i];
    stackSize *= 2;
    delete [] childStack;
    childStack = newStack;
  }
  childStack[top] = child(selectedChild);
  if (selectChild(which)) {
    return true;
  }
  top--;
  window()->redraw();
  return false;
}

bool Fl_MultiGroup::pushChild (Fl_Widget *w)
{
  int which = find(w);
  if (which < children())
    return pushChild(which);
  return false;
}

bool Fl_MultiGroup::popChild ()
{
  if (top < 0) 
    return false;
  selectChild(childStack[top--]);
  return true;
}

int Fl_MultiGroup::currentChild ()
{ return selectedChild; }

void Fl_MultiGroup::add (Fl_Widget &o)
{
  int  X, Y, W, H;
   X = x();  Y = y();  W = w();  H = h();

  if (children() > 0) {
    o.hide();
  } else {
    o.show();
    selectedChild = 0;
  }
  o.resize(X,Y,W,H);
  Fl_Group::add(o);
}

void Fl_MultiGroup::add (Fl_Widget *o)
{
  if (children() > 0) {
    o->hide();
  } else {
    o->show();
    selectedChild = 0;
  }
  Fl_Group::add(o);
}

void Fl_MultiGroup::insert (Fl_Widget &o, int number)
{
  int  X, Y, W, H;
  X = x();  Y = y();  W = w();  H = h();
  if (number == selectedChild)
    child(selectedChild)->hide();
  else
    o.hide();
  o.resize(X,Y,W,H);
  Fl_Group::insert(o,number);
  child(selectedChild)->show();
}


// Widget construction support

void Fl_MultiGroup::begin() 
 {
   Fl_Group::current(this);
 }

void Fl_MultiGroup::end()
 {
   int i, X, Y, W, H;
   X = x();  Y = y();  W = w();  H = h();
   Fl_Group::current(parent());

   for (i = 0 ; i < children(); i++) {
     child(i)->resize(X,Y,W,H);
     child(i)->hide();
   }
   selectChild(0);
   window()->redraw();
 }

