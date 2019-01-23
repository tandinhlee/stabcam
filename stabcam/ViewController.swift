//
//  ViewController.swift
//  stabcam
//
//  Created by Dinh Le on 1/12/19.
//  Copyright © 2019 Dinh Le. All rights reserved.
//

import UIKit
import Photos

class ViewController: UIViewController {
    
    @IBOutlet weak var videoView: UIImageView!
    var opencvWrapper: OpenCVWrapper!
    override func viewDidLoad() {
        super.viewDidLoad()
        opencvWrapper = OpenCVWrapper.init(withVideoParentView: videoView)
        // Do any additional setup after loading the view, typically from a nib.
    }
    @IBAction func startRecording(_ sender: Any) {
        opencvWrapper.actionStart(self);
    }
    
    
}
