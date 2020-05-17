# C++ implementation of Lexi

**Lexi** - WYSIWYG text redactor described in second chapter of [Design Patterns: Elements of Reusable Object-Oriented Software](https://en.wikipedia.org/wiki/Design_Patterns)
**Motivation** - I've been reading the book(as most of you) and faced the questions like "How this pattern is implemented?" or "Is there an open-source implementation of Lexi?". After searching internet for the answers I decided to implement it all by myself.
**Note**: Even though I've tried to implement stuff as described in the book, there are differences that I'll try to describe in the future.

## Pattern referencing
- [IGlyph](https://github.com/romaonishuk/LexI/blob/master/inc/glyphs/i_glyph.hpp) Abstract, base for all graphical units class. Described in Section 2.2 of GoF.
- [Composite](https://github.com/romaonishuk/LexI/blob/master/inc/glyphs/i_composite_glyph.hpp) Described in Section 2.2 of GoF.
- Strategy: basically any child glyph.
- [Decorator](https://github.com/romaonishuk/LexI/blob/master/inc/decorator.hpp) Note: currently, the scroller isn't a decorator as described in GoF due to implementation specific. Described in Section 2.4.
- [Abstract Factory](https://github.com/romaonishuk/LexI/blob/master/inc/window_system_factory.hpp) Described in Section 2.5.
- Bridge: [Base Window](https://github.com/romaonishuk/LexI/blob/master/inc/window.hpp), interface [WindowImpl](https://github.com/romaonishuk/LexI/blob/master/inc/window_impl.hpp) and its child [XWindowImpl](https://github.com/romaonishuk/LexI/blob/master/src/lexi_linux/inc/x_window_impl.hpp). Described in Section 2.6.
- [Command](https://github.com/romaonishuk/LexI/blob/master/inc/i_command.hpp) Described in Section 2.7.

*Note*: some of the patterns are WIP state and may distinguish from the implementation provided in GoF 

## In Progress
- Finishing text editor

## TODO List
- use only smart pointers
- migrate to a Windows(which includes expansion of WindowSystemFactory)
- add importatnt glyphs as text editor
- appropriate processing of all fonts(now only mono is supported)

## Discussion
- Pls let me know if I'm violating some property rights (romaonishuk@gmail.com)
