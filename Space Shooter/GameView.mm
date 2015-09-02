//
//  GameView.h
//  Space Shooter
//
//  Created by Karl Kirch on 8/10/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#import "GameView.h"
#import "GameManager.hpp"

@interface GameView () {
    NSTimer *sceneTimer;
    double lastFrameTime;
    CVDisplayLinkRef displayLink;
    GameManager gameManager;
    GameInput gameInput;
    NSLock *inputLock;
}

@end

@implementation GameView

@synthesize msPerFrame;

-(id)initWithCoder:(NSCoder *)coder
{
    if (self = [super initWithCoder:coder]) {
    }
    return self;
}

- (void)awakeFromNib {
    [super awakeFromNib];
    if (self) {
        inputLock = [[NSLock alloc] init];
        
        [self initGl];
        
        NSBundle* bundle = [NSBundle mainBundle];
        NSString* bundlePath = [bundle bundlePath];
        
        gameManager.init(std::string([bundlePath UTF8String]) + "/Contents/Resources/");
        
        // clean up game input
        for (int i = 0; i < 255; ++i) {
            gameInput.keys[i] = KS_UP;
        }
        
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
        CVDisplayLinkSetOutputCallback(displayLink, displayLinkCallback, (__bridge void *)(self));
        CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, [[self openGLContext] CGLContextObj], [[self pixelFormat] CGLPixelFormatObj]);
        CVDisplayLinkStart(displayLink);
        
        CVTime cvtime = CVDisplayLinkGetNominalOutputVideoRefreshPeriod(displayLink);
        msPerFrame = (double)cvtime.timeValue / (double)cvtime.timeScale * 1000;
        NSLog(@"Targeting %0.0f fps", ceil(1.0 / (msPerFrame / 1000.0)));
    }
}

- (void)initGl {
    NSOpenGLPixelFormatAttribute pfa[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        0
    };
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:pfa];
    if (!pf) {
        NSLog(@"Couldn't init opengl with double buffering, trying single buffer");
        NSOpenGLPixelFormatAttribute pfaFallback[] = {
            0
        };
        pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:pfaFallback];
    }
    if (!pf) {
        NSLog(@"Couldn't init opengl at all o_O, sorry :(");
        abort();
    }
    
    NSOpenGLContext *glContext = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    [self setPixelFormat:pf];
    [self setOpenGLContext:glContext];
}


-(void)prepareOpenGL
{
    [super prepareOpenGL];
    
    // make current context for use
    [[self openGLContext] makeCurrentContext];
    
    // lock context just in case something else is using it (shouldn't be though)
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
    glDisable(GL_DEPTH_TEST);
    
    // setup viewport for opengl
    NSSize size = [self frame].size;
    glViewport(0, 0, size.width, size.height);
    
    // set to use vsync (will default to 60hz in theory)
    GLint vsync = 1;
    [[self openGLContext] setValues:&vsync forParameter:NSOpenGLCPSwapInterval];
    
    // release lock on the opengl context
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

/**
 * Called when window is resized
 * This resizes the opengl context for us
 */
-(void)reshape
{
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
    // make current for subsequent opengl calls
    [[self openGLContext] makeCurrentContext];
    
    // setup viewport for opengl
    NSSize size = [self frame].size;
    if (size.width > 0 && size.height > 0) {
        glViewport(0, 0, size.width, size.height);
    }
    
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
    
    [self render];
}

CVReturn displayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *now,
                             const CVTimeStamp *outputTime, CVOptionFlags flagsIn,
                             CVOptionFlags *flagsOut, void *displayLinkContext)
{
    GameView *gameView = (__bridge GameView*)displayLinkContext;
    
    static CVTimeStamp lastRenderTime;
    uint64_t frameTime = now->hostTime - lastRenderTime.hostTime;
    lastRenderTime = *now;
    
    double frameTimeMs = (double)frameTime / (double)(1000 * 1000);
    
    if (frameTimeMs > 32) {
        frameTimeMs = 32;
    }
    
    // wrap our call to objc in an autorelease pool because the cvdisplaylink thread doesn't
    // have an autorelease pool by default
    @autoreleasepool {
        [gameView update:frameTimeMs];
        [gameView render];
    }
    return kCVReturnSuccess;
}

- (void)update:(double)dt {
    [inputLock lock];
    gameManager.update(dt, gameInput);
    [inputLock unlock];
}

/**
 * Render our color buffer via opengl to the screen
 */
- (void)render {
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
    // make current for subsequent opengl calls
    [[self openGLContext] makeCurrentContext];
    
    gameManager.render();
    
    glFlush();
    
    // we're double buffered so need to flush to screen
    [[self openGLContext] flushBuffer];
    
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)keyDown: (NSEvent *) event {
    [inputLock lock];
    gameInput.keys[[event keyCode]] = KS_DOWN;
    [inputLock unlock];
}

- (void)keyUp: (NSEvent *) event {
    [inputLock lock];
    gameInput.keys[[event keyCode]] = KS_UP;
    [inputLock unlock];
}

@end