#ifndef KURSOR_HPP
#define KURSOR_HPP

// OpenClass
#include <iobservr.hpp>     // IObserver
class IWindow;
class ISize;
class IPoint;
class IWindowHandle;
class IRectangle;


class _Export Kursor: public IObserver
{
  public:
    // constructor
    Kursor( IWindow * window );
    ~Kursor();

    // size, position, type
    Kursor & sizeTo( const ISize & size );
    Kursor & moveTo( const IPoint & point );
    enum KursorType {
      insert,
      overtype
    };
    Kursor & setType( KursorType kursorType );

    // create and show the cursor
    Kursor & update();
    Kursor & show( Boolean showKursor );

  protected:
    // IObserver
    virtual IObserver & dispatchNotificationEvent( const INotificationEvent &);

  private:
    // determine actual cursor size, based on type
    ISize cursorSize( const ISize & size ) const;

    // hide and destroy
    Kursor & destroy();

    // data
    IWindowHandle _windowHandle;
    IRectangle    _rect;
    KursorType    _kursorType;
    Boolean       _hasFocus;
    Boolean       _isCreated;
    Boolean       _attributeChanged;  // true if size or type changed
};

#endif
